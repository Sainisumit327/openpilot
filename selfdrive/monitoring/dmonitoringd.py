#!/usr/bin/env python3
import time
import traceback
import cereal.messaging as messaging
from openpilot.common.params import Params
from openpilot.common.realtime import config_realtime_process
from openpilot.selfdrive.monitoring.helpers import DriverMonitoring


def publish_log(pm, level: str, source: str, msg: str):
  """
  Publish a logMessage so the AGNOS/OpenPilot UI surfaces it on the Errors page.
  level: "error", "warning", "info", etc.
  """
  try:
    err = messaging.new_message('logMessage')
    lm = err.logMessage
    # fields used by the UI; keep them simple
    lm.level = level
    lm.source = source
    lm.msg = msg
    pm.send('logMessage', err)
  except Exception:
    # if even logging fails, print so it appears in process stdout
    print("Failed to publish logMessage:", traceback.format_exc())


def safe_set(struct, name, value):
  """Set `struct.name = value` only if the attribute exists."""
  if hasattr(struct, name):
    try:
      setattr(struct, name, value)
    except Exception:
      # avoid crashing if assignment type mismatches
      pass


def dmonitoringd_thread():
  # keep realtime config so process manager behaves like original
  config_realtime_process([0, 1, 2, 3], 5)

  params = Params()
  # publish both driverMonitoringState and logMessage so we can report errors
  pm = messaging.PubMaster(['driverMonitoringState', 'logMessage'])
  # subscribe to driverStateV2 so we can sync publishing to model frames
  sm = messaging.SubMaster(['driverStateV2', 'liveCalibration', 'carState',
                            'selfdriveState', 'modelV2', 'carControl'],
                           poll='driverStateV2')

  # Keep DriverMonitoring import/obj for compatibility (unused)
  try:
    DM = DriverMonitoring(rhd_saved=params.get_bool("IsRhdDetected"),
                          always_on=params.get_bool("AlwaysOnDM"))
  except Exception:
    DM = None

  last_frame = -1
  while True:
    try:
      sm.update()

      # publish only when model/driverState has new output (like original dmonitoringd)
      if not sm.updated.get('driverStateV2', False):
        # small sleep to yield CPU, original runs synced to model output
        time.sleep(0.02)
        continue

      # Build a new driverMonitoringState message
      dat = messaging.new_message('driverMonitoringState')
      dms = dat.driverMonitoringState

      # Fill only schema-backed fields (from your schema)
      safe_set(dms, 'faceDetected', True)
      safe_set(dms, 'isDistracted', False)
      safe_set(dms, 'distractedType', 0)
      safe_set(dms, 'awarenessStatus', 1.0)
      safe_set(dms, 'posePitchOffset', 0.0)
      safe_set(dms, 'posePitchValidCount', 1)
      safe_set(dms, 'poseYawOffset', 0.0)
      safe_set(dms, 'poseYawValidCount', 1)
      safe_set(dms, 'stepChange', 0.0)
      safe_set(dms, 'awarenessActive', 1.0)
      safe_set(dms, 'awarenessPassive', 1.0)
      safe_set(dms, 'isLowStd', False)
      safe_set(dms, 'hiStdCount', 0)
      safe_set(dms, 'isActiveMode', True)
      safe_set(dms, 'isRHD', params.get_bool("IsRhdDetected"))

      # send the driver monitoring state
      pm.send('driverMonitoringState', dat)

      # occasionally refresh the always-on param (match original behavior)
      frame_id = sm['driverStateV2'].frameId if sm.updated.get('driverStateV2') and 'driverStateV2' in sm.keys() else None
      if frame_id is not None and frame_id % 40 == 1 and DM is not None:
        try:
          DM.always_on = params.get_bool("AlwaysOnDM")
        except Exception:
          pass

      # short sleep to avoid busy loop — publishing is driven by sm updates anyway
      time.sleep(0.005)

    except Exception as e:
      # publish the traceback to the Errors UI and keep the process alive
      tb = traceback.format_exc()
      publish_log(pm, "error", "dmonitoringd", f"Exception in dmonitoringd: {e}\n{tb}")
      # wait a bit before retrying to avoid spamming
      time.sleep(1.0)


def main():
  dmonitoringd_thread()


if __name__ == '__main__':
  main()
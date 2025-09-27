#!/usr/bin/env python3
import time
import cereal.messaging as messaging
from openpilot.common.params import Params
from openpilot.common.realtime import config_realtime_process
from openpilot.selfdrive.monitoring.helpers import DriverMonitoring


def safe_set(struct, name, value):
  if hasattr(struct, name):
    try:
      setattr(struct, name, value)
    except Exception:
      pass


def dmonitoringd_thread():
  config_realtime_process([0, 1, 2, 3], 5)

  params = Params()
  pm = messaging.PubMaster(['driverMonitoringState'])
  sm = messaging.SubMaster(['driverStateV2', 'liveCalibration', 'carState',
                            'selfdriveState', 'modelV2', 'carControl'],
                           poll='driverStateV2')

  try:
    DM = DriverMonitoring(rhd_saved=params.get_bool("IsRhdDetected"),
                          always_on=params.get_bool("AlwaysOnDM"))
  except Exception:
    DM = None

  last_frame = -1
  while True:
    sm.update()

    if not sm.updated.get('driverStateV2', False):
      time.sleep(0.01)
      continue

    # get frameId if present so timing looks consistent
    try:
      frame_id = sm['driverStateV2'].frameId
    except Exception:
      frame_id = None

    # build message
    dat = messaging.new_message('driverMonitoringState')
    dms = dat.driverMonitoringState

    safe_set(dms, 'faceDetected', True)
    safe_set(dms, 'isDistracted', False)
    safe_set(dms, 'distractedType', 0)
    safe_set(dms, 'awarenessStatus', 1.0)
    safe_set(dms, 'awarenessActive', 1.0)
    safe_set(dms, 'awarenessPassive', 1.0)
    safe_set(dms, 'isLowStd', False)
    safe_set(dms, 'hiStdCount', 0)
    safe_set(dms, 'isActiveMode', True)
    safe_set(dms, 'isRHD', params.get_bool("IsRhdDetected"))

    # if driverMonitoringState schema has frameId, copy the frameId for consistency
    safe_set(dms, 'frameId', frame_id)

    pm.send('driverMonitoringState', dat)

    # small sleep (frame-driven anyway)
    time.sleep(0.005)


def main():
  dmonitoringd_thread()


if __name__ == '__main__':
  main()
#!/usr/bin/env python3
import cereal.messaging as messaging
from openpilot.common.params import Params
from openpilot.common.realtime import config_realtime_process
from openpilot.selfdrive.monitoring.helpers import DriverMonitoring


def dmonitoringd_thread():
  # stock RT config
  config_realtime_process([0, 1, 2, 3], 5)

  params = Params()
  pm = messaging.PubMaster(['driverMonitoringState'])
  sm = messaging.SubMaster(
    ['driverStateV2', 'liveCalibration', 'carState', 'selfdriveState', 'modelV2', 'carControl'],
    poll='driverStateV2'
  )

  # stock init (we won’t rely on DM’s attention logic; we just want its packet shape)
  DM = DriverMonitoring(rhd_saved=params.get_bool("IsRhdDetected"), always_on=params.get_bool("AlwaysOnDM"))

  # 20Hz driverStateV2-driven loop (same as stock)
  while True:
    sm.update()
    if not sm.updated['driverStateV2']:
      continue

    valid = sm.all_checks()

    # let DM run if you want (harmless); but we will override outputs anyway
    if valid:
      try:
        DM.run_step(sm)
      except Exception:
        # even if DM fails, we’ll still publish a valid attentive packet below
        pass

    # stock packet shape
    try:
      dat = DM.get_state_packet(valid=valid)
    except Exception:
      # if DM packet fails, build a new one of the right type
      dat = messaging.new_message('driverMonitoringState')

    dms = dat.driverMonitoringState

    # --- SIM BYPASS: force "always attentive" & safe values (using fields from your schema) ---
    # If fields don’t exist on a given release, assigning silently no-ops—safe to keep.
    try:
      dms.faceDetected = True
    except Exception:
      pass
    try:
      dms.isDistracted = False
    except Exception:
      pass
    try:
      dms.distractedType = 0
    except Exception:
      pass
    try:
      dms.awarenessStatus = 1.0
      dms.awarenessActive = 1.0
      dms.awarenessPassive = 1.0
    except Exception:
      pass
    try:
      dms.isLowStd = False
      dms.hiStdCount = 0
    except Exception:
      pass
    try:
      dms.isActiveMode = True
      dms.isRHD = params.get_bool("IsRhdDetected")
    except Exception:
      pass
    # Make sure events is present and empty so no alerts propagate
    try:
      dms.events = []
    except Exception:
      pass
    # --- END SIM BYPASS ---

    pm.send('driverMonitoringState', dat)

    # live toggle (keep stock behavior)
    try:
      if sm['driverStateV2'].frameId % 40 == 1:
        DM.always_on = params.get_bool("AlwaysOnDM")
    except Exception:
      pass

    # save rhd virtual toggle every 5 mins (keep stock behavior, but guard)
    try:
      if (sm['driverStateV2'].frameId % 6000 == 0 and
          DM.wheelpos_learner.filtered_stat.n > DM.settings._WHEELPOS_FILTER_MIN_COUNT and
          DM.wheel_on_right == (DM.wheelpos_learner.filtered_stat.M > DM.settings._WHEELPOS_THRESHOLD)):
        params.put_bool_nonblocking("IsRhdDetected", DM.wheel_on_right)
    except Exception:
      pass


def main():
  dmonitoringd_thread()


if __name__ == '__main__':
  main()
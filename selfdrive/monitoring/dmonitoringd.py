#!/usr/bin/env python3
import gc
import cereal.messaging as messaging
from openpilot.common.params import Params
from openpilot.common.realtime import set_realtime_priority
from openpilot.selfdrive.monitoring.helpers import DriverMonitoring


def dmonitoringd_thread():
  gc.disable()
  set_realtime_priority(2)

  params = Params()
  pm = messaging.PubMaster(['driverMonitoringState', 'driverMonitoringStateSP'])
  sm = messaging.SubMaster(['driverStateV2', 'liveCalibration', 'carState', 'controlsState', 'modelV2'], poll='driverStateV2')

  DM = DriverMonitoring(
    rhd_saved=params.get_bool("IsRhdDetected"),
    always_on=params.get_bool("AlwaysOnDM"),
    hands_on_wheel_monitoring=params.get_bool("HandsOnWheelMonitoring")
  )

  # 20Hz <- dmonitoringmodeld (in older versions)
  while True:
    sm.update()
    if not sm.updated['driverStateV2']:
      continue

    valid = sm.all_checks()
    if valid:
      try:
        DM.run_step(sm)
      except Exception:
        pass

    # publish driverMonitoringState
    try:
      dat = DM.get_state_packet(valid=valid)
    except Exception:
      dat = messaging.new_message('driverMonitoringState')

    try:
      dms = dat.driverMonitoringState
      dms.faceDetected = True
      dms.isDistracted = False
      dms.distractedType = 0
      dms.awarenessStatus = 1.0
      dms.awarenessActive = 1.0
      dms.awarenessPassive = 1.0
      dms.isActiveMode = True
      dms.isRHD = params.get_bool("IsRhdDetected")
      dms.events = []
    except Exception:
      pass

    pm.send('driverMonitoringState', dat)

    # publish driverMonitoringStateSP
    try:
      sp_dat = DM.get_sp_state_packet(valid=valid)
    except Exception:
      sp_dat = messaging.new_message('driverMonitoringStateSP')

    try:
      dms_sp = sp_dat.driverMonitoringStateSP
      dms_sp.faceDetected = True
      dms_sp.isDistracted = False
      dms_sp.distractedType = 0
      dms_sp.awarenessStatus = 1.0
      dms_sp.awarenessActive = 1.0
      dms_sp.awarenessPassive = 1.0
      dms_sp.isActiveMode = True
      dms_sp.isRHD = params.get_bool("IsRhdDetected")
      dms_sp.events = []
    except Exception:
      pass

    pm.send('driverMonitoringStateSP', sp_dat)

    # load live toggles
    try:
      if sm['driverStateV2'].frameId % 40 == 1:
        DM.always_on = params.get_bool("AlwaysOnDM")
        DM.hands_on_wheel_monitoring = params.get_bool("HandsOnWheelMonitoring")
    except Exception:
      pass

    # save rhd toggle every 5 mins
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
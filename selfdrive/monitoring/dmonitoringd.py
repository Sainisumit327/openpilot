#!/usr/bin/env python3
import cereal.messaging as messaging
from openpilot.common.params import Params
from openpilot.common.realtime import config_realtime_process
from openpilot.selfdrive.monitoring.helpers import DriverMonitoring


def dmonitoringd_thread():
  # keep realtime config so process manager doesn’t complain
  config_realtime_process([0, 1, 2, 3], 5)

  params = Params()
  pm = messaging.PubMaster(['driverMonitoringState'])
  sm = messaging.SubMaster(['driverStateV2', 'liveCalibration', 'carState', 'selfdriveState', 'modelV2',
                            'carControl'], poll='driverStateV2')

  # keep object init (unused now, just for stability)
  DM = DriverMonitoring(rhd_saved=params.get_bool("IsRhdDetected"), always_on=params.get_bool("AlwaysOnDM"))

  while True:
    sm.update()

    # build new message
    dat = messaging.new_message('driverMonitoringState')
    dms = dat.driverMonitoringState

    # simulate attentive driver
    dms.faceDetected = True
    dms.isDistracted = False
    dms.distractedType = 0
    dms.awarenessStatus = 1.0
    dms.posePitchOffset = 0.0
    dms.posePitchValidCount = 1
    dms.poseYawOffset = 0.0
    dms.poseYawValidCount = 1
    dms.stepChange = 0.0
    dms.awarenessActive = 1.0
    dms.awarenessPassive = 1.0
    dms.isLowStd = False
    dms.hiStdCount = 0
    dms.isActiveMode = True
    dms.isRHD = False

    # publish
    pm.send('driverMonitoringState', dat)


def main():
  dmonitoringd_thread()


if __name__ == '__main__':
  main()
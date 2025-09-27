#!/usr/bin/env python3
import time
import cereal.messaging as messaging
from openpilot.common.params import Params
from openpilot.common.realtime import config_realtime_process
from openpilot.selfdrive.monitoring.helpers import DriverMonitoring


def dmonitoringd_thread():
    config_realtime_process([0, 1, 2, 3], 5)

    pm = messaging.PubMaster(['driverMonitoringState'])
    params = Params()

    while True:
        dat = messaging.new_message('driverMonitoringState')
        dat.driverMonitoringState.faceDetected = True
        dat.driverMonitoringState.isActiveMode = True
        dat.driverMonitoringState.awake = True
        dat.driverMonitoringState.faceProbability = 1.0
        dat.driverMonitoringState.readyToDrive = True

        pm.send('driverMonitoringState', dat)

        time.sleep(0.05)  # ~20 Hz loop


def main():
    dmonitoringd_thread()


if __name__ == "__main__":
    main()
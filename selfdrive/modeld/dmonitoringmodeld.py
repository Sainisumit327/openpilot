#!/usr/bin/env python3
"""
Minimal fake dmonitoringmodeld for simulation.

Publishes a simple driverStateV2 message at ~20Hz with an incrementing frameId
so downstream dmonitoringd can operate as if a model produced frames.

Safe: it checks attributes before assignment to avoid capnp AttributeError.
Simulation-only: publishes synthetic "good" frames.
"""
import time
import traceback
import cereal.messaging as messaging
from openpilot.common.realtime import config_realtime_process


def safe_set(struct, name, value):
  if hasattr(struct, name):
    try:
      setattr(struct, name, value)
    except Exception:
      pass


def publish_log(pm, level: str, source: str, msg: str):
  # helper in case you later want logs (does not require logmessaged here)
  try:
    if 'logMessage' not in pm.__dict__:
      # pm was created with only driverStateV2; to send logMessage you'd need it in PubMaster
      return
  except Exception:
    return


def dmonitoringmodeld_thread():
  # match original behavior (real-time priority)
  config_realtime_process([0, 1, 2, 3], 5)

  pm = messaging.PubMaster(['driverStateV2'])
  frame_id = 0

  while True:
    try:
      # create a new driverStateV2 message
      dat = messaging.new_message('driverStateV2')

      # Many driverStateV2 schemas contain a top-level 'frameId' (used by dmonitoringd)
      # Use safe_set to avoid AttributeError if field differs
      safe_set(dat, 'frameId', frame_id)

      # If the schema contains a nested struct for driver state, we can set a few fields safely.
      # e.g. dat.driverState.someField — but to be maximally safe, set only top-level fields.
      # If there are commonly expected subfields, set them if present:
      if hasattr(dat, 'driverState'):
        ds = dat.driverState
        safe_set(ds, 'faceProb', 1.0)
        safe_set(ds, 'facePoseValid', True)

      # Publish
      pm.send('driverStateV2', dat)

      # increment frame id (wrap-around safe)
      frame_id = (frame_id + 1) & 0x7FFFFFFF

      # ~20Hz
      time.sleep(0.05)

    except Exception as e:
      # print trace to stdout (tmux will show it)
      print("dmonitoringmodeld exception:", e)
      print(traceback.format_exc())
      # backoff a bit and continue so the process doesn't die
      time.sleep(1.0)


def main():
  dmonitoringmodeld_thread()


if __name__ == '__main__':
  main()
# CMake generated Testfile for 
# Source directory: /home/fengwork/AcuteAngle/Acute-Angle-Chain/contracts/identity
# Build directory: /home/fengwork/AcuteAngle/Acute-Angle-Chain/build/contracts/identity
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(validate_identity_abi "/home/fengwork/AcuteAngle/Acute-Angle-Chain/build/scripts/abi_is_json.py" "/home/fengwork/AcuteAngle/Acute-Angle-Chain/contracts/identity/identity.abi")
subdirs(test)

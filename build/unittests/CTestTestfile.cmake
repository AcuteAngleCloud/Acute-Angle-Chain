# CMake generated Testfile for 
# Source directory: /home/fengwork/AcuteAngle/Acute-Angle-Chain/unittests
# Build directory: /home/fengwork/AcuteAngle/Acute-Angle-Chain/build/unittests
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(unit_test_binaryen "/home/fengwork/AcuteAngle/Acute-Angle-Chain/build/unittests/unit_test.exe" "-t" "!wasm_tests/weighted_cpu_limit_tests" "--report_level=detailed" "--color_output" "--" "--binaryen")
add_test(unit_test_wavm "/home/fengwork/AcuteAngle/Acute-Angle-Chain/build/unittests/unit_test.exe" "-t" "!wasm_tests/weighted_cpu_limit_tests" "--report_level=detailed" "--color_output" "--catch_system_errors=no" "--" "--wavm")
subdirs(contracts)

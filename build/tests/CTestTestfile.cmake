# CMake generated Testfile for 
# Source directory: /home/fengwork/AcuteAngle/Acute-Angle-Chain/tests
# Build directory: /home/fengwork/AcuteAngle/Acute-Angle-Chain/build/tests
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(plugin_test "/home/fengwork/AcuteAngle/Acute-Angle-Chain/build/tests/plugin_test.exe" "--report_level=detailed" "--color_output")
add_test(nodaac_run_test "tests/nodaac_run_test.py" "-v" "--clean-run" "--dump-error-detail")
set_tests_properties(nodaac_run_test PROPERTIES  WORKING_DIRECTORY "/home/fengwork/AcuteAngle/Acute-Angle-Chain/build")
add_test(p2p_dawn515_test "tests/p2p_tests/dawn_515/test.sh")
set_tests_properties(p2p_dawn515_test PROPERTIES  WORKING_DIRECTORY "/home/fengwork/AcuteAngle/Acute-Angle-Chain/build")
add_test(distributed-transactions-test "tests/distributed-transactions-test.py" "-d" "2" "-p" "1" "-n" "4" "-v" "--clean-run" "--dump-error-detail")
set_tests_properties(distributed-transactions-test PROPERTIES  WORKING_DIRECTORY "/home/fengwork/AcuteAngle/Acute-Angle-Chain/build")
add_test(restart-scenarios-test-resync "tests/restart-scenarios-test.py" "-c" "resync" "-p4" "-v" "--clean-run" "--dump-error-details")
set_tests_properties(restart-scenarios-test-resync PROPERTIES  WORKING_DIRECTORY "/home/fengwork/AcuteAngle/Acute-Angle-Chain/build")
add_test(restart-scenarios-test-hard_replay "tests/restart-scenarios-test.py" "-c" "hardReplay" "-p4" "-v" "--clean-run" "--dump-error-details")
set_tests_properties(restart-scenarios-test-hard_replay PROPERTIES  WORKING_DIRECTORY "/home/fengwork/AcuteAngle/Acute-Angle-Chain/build")
add_test(restart-scenarios-test-none "tests/restart-scenarios-test.py" "-c" "none" "--kill-sig" "term" "-p4" "-v" "--clean-run" "--dump-error-details")
set_tests_properties(restart-scenarios-test-none PROPERTIES  WORKING_DIRECTORY "/home/fengwork/AcuteAngle/Acute-Angle-Chain/build")
add_test(validate_dirty_db_test "tests/validate-dirty-db.py" "-v" "--clean-run" "--dump-error-detail")
set_tests_properties(validate_dirty_db_test PROPERTIES  WORKING_DIRECTORY "/home/fengwork/AcuteAngle/Acute-Angle-Chain/build")

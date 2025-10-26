
TEST_SERVER_PID=""

cleanup() {
    echo "Cleaning up..."
    if [ ! -z "$TEST_SERVER_PID" ]; then
        echo "Stopping test server (PID: $TEST_SERVER_PID)"
        sudo kill $TEST_SERVER_PID
    fi
    echo "✓ Cleanup completed"
}

trap cleanup EXIT INT TERM

echo
echo "[1] Starting test-server..."
echo

./tests/test_server/main.exe &
TEST_SERVER_PID=$!
sleep 2

echo
echo "[2] Starting programm..."
echo

if ! ./build/main.exe urls.txt downloads 2; then
    echo "Starting program failed"
    exit 1
else
    echo "✓ Program successfully started"
fi

echo
echo "[3] Cheching tests results..."
echo

if [ -f "./downloads/i_m_name_form_header_from_test_server" ]; then
    echo "✓ i_m_name_form_header_from_test_server exist"
else
    echo "Error: File i_m_name_form_header_from_test_server not exist!"
    exit 1
fi

if [ -f "./downloads/2_i_m_name_form_header_from_test_server" ]; then
    echo "✓ 2_i_m_name_form_header_from_test_server exist"
else
    echo "Error: File 2_i_m_name_form_header_from_test_server not exist!"
    exit 1
fi

if grep -q "I'm bubilda!" "./downloads/i_m_name_form_header_from_test_server" || grep -q "I'm pipapupa!" "./2_downloads/i_m_name_form_header_from_test_server"; then
    echo "✓ The contents of the files are correct"
else
    echo "Error: Other file contents were expected"
    exit 1
fi

echo "✓ All tests successful!"
#include "config.h"

#include "test/src/test_command_dynamic.h"

#include "control.h"
#include "globals.h"
#include "rpc/parse_commands.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestCommandDynamic);

void initialize_command_dynamic();
void initialize_command_ui();

void
TestCommandDynamic::setUp() {
  m_test_main_thread = TestMainThread::create();
  m_test_main_thread->init_thread();

  if (rpc::commands.find("method.insert") == rpc::commands.end()) {
    setlocale(LC_ALL, "");
    // cachedTime = rak::timer::current();
    control = new Control;

    initialize_command_dynamic();
    initialize_command_ui();
  }
}

void
TestCommandDynamic::tearDown() {
  m_test_main_thread.reset();
}

void
TestCommandDynamic::test_basics() {
  rpc::commands.call_command("method.insert.value", rpc::create_object_list("test_basics.1", int64_t(1)));
  CPPUNIT_ASSERT(rpc::commands.call_command("test_basics.1", torrent::Object()).as_value() == 1);
}

void
TestCommandDynamic::test_get_set() {
  rpc::commands.call_command("method.insert.simple", rpc::create_object_list("test_get_set.1", "cat=1"));
  CPPUNIT_ASSERT(rpc::commands.call_command("test_get_set.1", torrent::Object()).as_string() == "1");
  CPPUNIT_ASSERT(rpc::commands.call_command("method.get", "test_get_set.1").as_string() == "cat=1");

  rpc::commands.call_command("method.set", rpc::create_object_list("test_get_set.1", "cat=2"));
  CPPUNIT_ASSERT(rpc::commands.call_command("method.get", "test_get_set.1").as_string() == "cat=2");
}

void
TestCommandDynamic::test_old_style() {
  rpc::commands.call_command("method.insert", rpc::create_object_list("test_old_style.1", "value", int64_t(1)));
  CPPUNIT_ASSERT(rpc::commands.call_command("test_old_style.1", torrent::Object()).as_value() == 1);

  rpc::commands.call_command("method.insert", rpc::create_object_list("test_old_style.2", "bool", int64_t(5)));
  CPPUNIT_ASSERT(rpc::commands.call_command("test_old_style.2", torrent::Object()).as_value() == 1);

  rpc::commands.call_command("method.insert", rpc::create_object_list("test_old_style.3", "string", "test.2"));
  CPPUNIT_ASSERT(rpc::commands.call_command("test_old_style.3", torrent::Object()).as_string() == "test.2");

  rpc::commands.call_command("method.insert", rpc::create_object_list("test_old_style.4", "simple", "cat=test.3"));
  CPPUNIT_ASSERT(rpc::commands.call_command("test_old_style.4", torrent::Object()).as_string() == "test.3");
}

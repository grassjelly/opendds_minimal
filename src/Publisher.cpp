/*
 *
 *
 * Distributed under the OpenDDS License.
 * See: http://www.opendds.org/license.html
 */

#include "boilerplate/Boilerplate.h"
#include <dds/DCPS/Service_Participant.h>
#include <model/Sync.h>
#include <stdexcept>
#include "test/test.h"

#include "dds/DCPS/StaticIncludes.h"

using namespace examples::boilerplate;

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try {
    // Initialize DomainParticipantFactory, handling command line args
    DDS::DomainId_t domain_id = 42;
    const char* topic_name{"test1"};

    DDS::DomainParticipantFactory_var dpf =
      TheParticipantFactoryWithArgs(argc, argv);

    // Create domain participant
    DDS::DomainParticipant_var participant = createParticipant(dpf, domain_id);

    // Register type support and create topic
    DDS::Topic_var topic = createTopic(participant, topic_name);

    // Create publisher
    DDS::Publisher_var publisher = createPublisher(participant);

    // Create data writer for the topic
    DDS::DataWriter_var writer = createDataWriter(publisher, topic);

    // Safely downcast data writer to type-specific data writer
    Template::MessageDataWriter_var msg_writer = narrow(writer);

    {
      // Block until Subscriber is available
      OpenDDS::Model::WriterSync ws(writer);

      // Initialize samples
      Template::Message message;
      message.data = TEST_VAR;

      // Override message count
      int msg_count = 10;
      if (argc > 1) {
        msg_count = ACE_OS::atoi(argv[1]);
      }

      for (int i = 0; i < msg_count; ++i) {
        // Publish the message
        DDS::ReturnCode_t error = msg_writer->write(message,
                                                    DDS::HANDLE_NIL);
        if (error != DDS::RETCODE_OK) {
          ACE_ERROR((LM_ERROR,
                     ACE_TEXT("ERROR: %N:%l: main() -")
                     ACE_TEXT(" write returned %d!\n"), error));
        }

        // Prepare next sample
        // ++message.count;
        // ++message.subject_id;
      }

      // End of WriterSync scope - block until messages acknowledged
    }

    // Clean-up!
    cleanup(participant, dpf);

  } catch (const CORBA::Exception& e) {
    e._tao_print_exception("Exception caught in main():");
    return -1;
  } catch (std::runtime_error& err) {
    ACE_ERROR_RETURN((LM_ERROR, ACE_TEXT("ERROR: main() - %s\n"),
                      err.what()), -1);
  } catch (std::string& msg) {
    ACE_ERROR_RETURN((LM_ERROR, ACE_TEXT("ERROR: main() - %s\n"),
                      msg.c_str()), -1);
  }

  return 0;
}

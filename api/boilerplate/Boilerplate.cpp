/*
 *
 *
 * Distributed under the OpenDDS License.
 * See: http://www.opendds.org/license.html
 */

#include "boilerplate/Boilerplate.h"
#include <dds/DCPS/Marked_Default_Qos.h>
#include <dds/DCPS/Service_Participant.h>

namespace examples { namespace boilerplate {

DDS::DomainParticipant_var
createParticipant(DDS::DomainParticipantFactory_var dpf, DDS::DomainId_t domain_id)
{
  // Create DomainParticipant
  DDS::DomainParticipant_var participant =
    dpf->create_participant(domain_id, // made-up domain ID
                            PARTICIPANT_QOS_DEFAULT,
                            0,  // no listener
                            OpenDDS::DCPS::DEFAULT_STATUS_MASK);

  // Check for failure
  if (!participant) {
    throw std::string("failed to create domain participant");
  }
  return participant;
}

DDS::Topic_var
createTopic(DDS::DomainParticipant_var participant, const char* topic_name)
{
  // Register TypeSupport (Template::Message)
  Template::MessageTypeSupport_var ts =
    new Template::MessageTypeSupportImpl;

  if (ts->register_type(participant, "") != DDS::RETCODE_OK) {
    throw std::string("failed to register type support");
  }

  // Create Topic (Movie Discussion List)
  CORBA::String_var type_name = ts->get_type_name();
  DDS::Topic_var topic =
    participant->create_topic(topic_name,
                              type_name,
                              TOPIC_QOS_DEFAULT,
                              0,
                              OpenDDS::DCPS::DEFAULT_STATUS_MASK);

  // Check for failure
  if (!topic) {
    throw std::string("failed to create topic");
  }
  return topic;
}

DDS::Publisher_var
createPublisher(DDS::DomainParticipant_var participant)
{
  // Create Publisher
  DDS::Publisher_var publisher =
    participant->create_publisher(
        PUBLISHER_QOS_DEFAULT,
        0,
        OpenDDS::DCPS::DEFAULT_STATUS_MASK);

  // Check for failure
  if (!publisher) {
    throw std::string("failed to create publisher");
  }
  return publisher;
}

DDS::Subscriber_var
createSubscriber(DDS::DomainParticipant_var participant)
{
  // Create Subscriber
  DDS::Subscriber_var subscriber =
      participant->create_subscriber(
          SUBSCRIBER_QOS_DEFAULT,
          0,
          OpenDDS::DCPS::DEFAULT_STATUS_MASK);

  // Check for failure
  if (!subscriber) {
    throw std::string("failed to create subscriber");
  }

  return subscriber;
}

DDS::DataWriter_var
createDataWriter(
  DDS::Publisher_var publisher,
  DDS::Topic_var topic)
{
  // Create DataWriter
  DDS::DataWriter_var writer =
    publisher->create_datawriter(topic,
                                 DATAWRITER_QOS_DEFAULT,
                                 0,
                                 OpenDDS::DCPS::DEFAULT_STATUS_MASK);

  // Check for failure
  if (!writer) {
    throw std::string("failed to create data writer");
  }

  return writer;
}

DDS::DataReader_var
createDataReader(
  DDS::Subscriber_var subscriber,
  DDS::Topic_var topic,
  DDS::DataReaderListener_var listener)
{
  // Create DataReader
  DDS::DataReader_var reader =
    subscriber->create_datareader(topic,
                                  DATAREADER_QOS_DEFAULT,
                                  listener,
                                  OpenDDS::DCPS::DEFAULT_STATUS_MASK);

  // Check for failure
  if (!reader) {
    throw std::string("failed to create data reader");
  }

  return reader;
}

Template::MessageDataWriter_var
narrow(DDS::DataWriter_var writer)
{
  // Safe cast to type-specific data writer
  Template::MessageDataWriter_var message_writer =
    Template::MessageDataWriter::_narrow(writer);

  // Check for failure
  if (!message_writer) {
    throw std::string("failed to narrow data writer");
  }

  return message_writer;
}

Template::MessageDataReader_var
narrow(DDS::DataReader_var reader)
{
  // Safe cast to type-specific data reader
  Template::MessageDataReader_var message_reader =
    Template::MessageDataReader::_narrow(reader);

  // Check for failure
  if (!message_reader) {
    throw std::string("failed to narrow data reader");
  }

  return message_reader;
}

void
cleanup(
  DDS::DomainParticipant_var participant,
  DDS::DomainParticipantFactory_var dpf)
{
  // Delete any topics, publishers and subscribers owned by participant
  participant->delete_contained_entities();
  // Delete participant itself
  dpf->delete_participant(participant);
  // Shut down info repo connection
  TheServiceParticipant->shutdown();
}

} } // End namespaces

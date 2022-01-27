#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <cctype>
#include <thread>
#include <chrono>
#include <mqtt/async_client.h>

template <typename Message>
class callback : public virtual mqtt::callback,
					public virtual mqtt::iaction_listener

{
	// Counter for the number of connection retries
	int nretry_;
	// The MQTT client
	mqtt::async_client& cli_;
	// Options to use if we need to reconnect
	mqtt::connect_options& connOpts_;
	// An action listener to display the result of actions.
	action_listener subListener_;

	// This deomonstrates manually reconnecting to the broker by calling
	// connect() again. This is a possibility for an application that keeps
	// a copy of it's original connect_options, or if the app wants to
	// reconnect with different options.
	// Another way this can be done manually, if using the same options, is
	// to just call the async_client::reconnect() method.
	void reconnect() {
		std::this_thread::sleep_for(std::chrono::milliseconds(2500));
		try {
			cli_.connect(connOpts_, nullptr, *this);
		}
		catch (const mqtt::exception& exc) {
			std::cerr << "Error: " << exc.what() << std::endl;
			exit(1);
		}
	}

	// Re-connection failure
	void on_failure(const mqtt::token& tok) override {
		std::cout << "MQTT Connection attempt failed" << std::endl;
		if (++nretry_ > N_RETRY_ATTEMPTS)
			exit(1);
		reconnect();
	}

	// (Re)connection success
	// Either this or connected() can be used for callbacks.
	void on_success(const mqtt::token& tok) override {}

	// (Re)connection success
	void connected(const std::string& cause) override {
		cli_.subscribe(TOPIC, QOS, nullptr, subListener_);
	}

	// Callback for when the connection is lost.
	// This will initiate the attempt to manually reconnect.
	void connection_lost(const std::string& cause) override {
		std::cout << "\nConnection lost" << std::endl;
		if (!cause.empty())
			std::cout << "\tcause: " << cause << std::endl;

		std::cout << "Reconnecting..." << std::endl;
		nretry_ = 0;
		reconnect();
	}

	// Callback for when a message arrives.
	void message_arrived(mqtt::const_message_ptr msg) override {
		std::cout << "Message arrived" << std::endl;
		std::cout << "\ttopic: '" << msg->get_topic() << "'" << std::endl;
		Message message;
		std::string string_msg = msg->get_payload();
		bool success = location.ParseFromString(string_msg);
		if (success)

		else std::cout << "failure" <<std::endl;
	}

	void delivery_complete(mqtt::delivery_token_ptr token) override {}

public:
	callback(mqtt::async_client& cli, mqtt::connect_options& connOpts, )
				: nretry_(0), cli_(cli), connOpts_(connOpts), subListener_("Subscription") {}
};
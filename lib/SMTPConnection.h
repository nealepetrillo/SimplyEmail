/*
 * SMTPConnection.h
 *
 *  Created on: Aug 20, 2014
 *      Author: nap
 */

#ifndef SMTPCONNECTION_H_
#define SMTPCONNECTION_H_

#include <string>
#include <sstream>
#include <stdexcept>
#include <stdio.h>
#include <fstream>
#include <curl/curl.h>
#include "Email.h"

namespace SimplyEmail {

//TODO Create a debug function to take advantage of the CURLOPT_DEBUGFUNCTION option.

class SMTPConnection {
public:
	static const int OPENING_CONNECTION;				/// Status indicating that the object is attempting to open a connection to the SMTP server
	static const int CONNECTION_OPEN;					/// Status indication that the object has connected to the SMTP server
	static const int SENDING_DATA;						/// Status indicating that the object is attempting to send data
	static const int SENDING_COMPLETE;					/// Status indicating that the object has finished sending the data
	static const int CLOSING_CONNECTION;				/// Status indicating that the object is attempting to close the connection to the SMTP server
	static const int CONNECTION_CLOSED;					/// Status indicating that the object has closed the connection

	/**
	 * \brief Creates a default connection to an SMTP server
	 *
	 * \details Creates an empty SMTP connection.
	 *
	 * \return void
	 */
	SMTPConnection();

	/**
	 * \brief Parameterized SMTP connection constructor
	 *
	 * \details Creates an SMTP connection using provided parameters
	 *
	 * \param[in] address The address of the SMTP server. Must be preceded by smtp:// and should include port number.
	 * \param[in] username The username to access the SMTP server with
	 * \param[in] password The password to access the SMTP server with
	 *
	 * \return void
	 */
	SMTPConnection(std::string address,std::string username, std::string password);

	/**
	 * \brief SMTP copy constructor
	 *
	 * \details Creates a new SMTP connection instance from a previous instance
	 *
	 * \return void
	 */
	SMTPConnection(SMTPConnection& other);

	/**
	 * \brief SMTP destructor
	 *
	 * \details Closes any open SMTP connections and destroys the object
	 *
	 * \return void
	 */
	~SMTPConnection();

	/**
	 * \brief Initializes the SMTP object using parameterized data
	 *
	 * \details Populates SMTP object members using provided data
	 *
	 * \return void
	 */
	void initialize(std::string address,std::string username, std::string password);

	/**
	 * \brief Disconnects from an SMTP server
	 *
	 * \details Closes the connection between the object and an SMTP server.
	 *
	 * \return void
	 */
	void disconnect();

	/**
	 * \brief Sends an email
	 *
	 * \details Sends the email whos reference is passed. Updates the send status bit.
	 *
	 * \param[in] email A reference to the email to be sent.
	 *
	 * \return void
	 */
	void send(SimplyEmail::Email &email);

	/**
	 * \brief Gets the current staus of sending
	 *
	 * \details Gets the current status of the sending process. //TODO Document possible return variables defined above.
	 *
	 * \return int The status of the SMTP connection.
	 */
	int getStatus();

	//TODO Document getteres and setters
	std::string getAddress();
	std::string getUsername();
	std::string getPassword();

private:
	CURL* curl;				/// The connection to the CURL interface
	int res;				/// The current status of CURL

	std::string address;	/// The address of the SMTP server
	std::string username;	/// The username to connect to the SMTP server
	std::string password;	/// The password to connect to the SMTP server

	void checkConnection(unsigned int toCheck);


};

} /* namespace SimplyEmail */

#endif /* SMTPCONNECTION_H_ */

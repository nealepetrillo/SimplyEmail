/**
 * \file SMTPConnection.cpp
 *
 * \brief Implementation file for SMTPConnection object
 *
 * \author Neale Petrillo <neale.a.petrillo@gmail.com>
 * \copyright Neale Petrillo, 2015
 */

#include "../lib/SMTPConnection.h"

namespace SimplyEmail {

const int SMTPConnection::OPENING_CONNECTION = 5;
const int SMTPConnection::CONNECTION_OPEN = 4;
const int SMTPConnection::SENDING_DATA = 3;
const int SMTPConnection::SENDING_COMPLETE = 2;
const int SMTPConnection::CLOSING_CONNECTION = 1;
const int SMTPConnection::CONNECTION_CLOSED = 0;

SMTPConnection::SMTPConnection() {
	this->curl = NULL;

	//Initialize the SMTP connection with empty strings.
	this->initialize("","","");

}

SMTPConnection::SMTPConnection(std::string address,std::string username, std::string password) {
	this->curl = NULL;

	this->initialize(address,username,password);
}

SMTPConnection::SMTPConnection(SMTPConnection& other){
	this->initialize(other.getAddress(), other.getUsername(), other.getPassword());
}

SMTPConnection::~SMTPConnection() {
	//Disconnect the object
	this->disconnect();
}

void SMTPConnection::initialize(const std::string &address, const std::string &username, const std::string &password){
	//Disconnect any previous sessions
	if(this->curl) {
		this->disconnect();
	}

	//Set connection parameters
	this->address = address;
	this->username = username;
	this->password = password;

	//Get a CURL instance
	this->curl = curl_easy_init();

	// Make sure that CURL was started
	if(!this->curl) {
		throw std::runtime_error("Error connecting to SMTP server: CURL did not start");
	}

	this->res = this->CONNECTION_CLOSED;

	//Set the CURL options
	curl_easy_setopt(this->curl,CURLOPT_URL, this->address.c_str());	// Set the address of the SMTP server. Server name must specify smtp://
	curl_easy_setopt(this->curl, CURLOPT_USERNAME, this->username.c_str());		// Set the username for authentication
    curl_easy_setopt(this->curl, CURLOPT_PASSWORD, this->password.c_str());		// Set the password for authentication

	curl_easy_setopt(this->curl,CURLOPT_SSL_VERIFYPEER, 1L);			// Force verification of peers
	curl_easy_setopt(this->curl,CURLOPT_SSL_VERIFYHOST, 1L);			// Force verification of server
	curl_easy_setopt(this->curl,CURLOPT_UPLOAD, 1L);					// Set the upload flag
	curl_easy_setopt(this->curl, CURLOPT_VERBOSE, 1L);					//TODO Verbose mode is only for testing. Remove this for production.

}

void SMTPConnection::disconnect(){
	this->res = this->CLOSING_CONNECTION;

	if(this->curl) {
		curl_easy_cleanup(this->curl);
	}

	this->res = this->CONNECTION_CLOSED;
}

void SMTPConnection::send(SimplyEmail::Email &email){

	//Check to make sure that the connection is open
	if(this->curl) {

		//Set status
		this->res = this->OPENING_CONNECTION;

		//Create an  empty recipients list
		struct curl_slist *recipients = NULL;

		//Set CURL options
		curl_easy_setopt(this->curl,CURLOPT_MAIL_FROM,email.getFrom().c_str());


		//Add recipients
		if(email.getRecipientNumber() > 0){
			for(unsigned int i=0; i < email.getRecipientNumber(); i++){
				recipients = curl_slist_append(recipients, email.getRecipient(i).c_str());
			}
		}
		else {
			throw std::runtime_error("Error connecting to SMTP server: No recipients defined in email");
		}

		//Add CC
		for(unsigned int i=0; i < email.getCCNumber(); i++){
			recipients = curl_slist_append(recipients, email.getCC(i).c_str());
		}

		//Add BCC
		for(unsigned int i=0; i < email.getBCCNumber(); i++){
			recipients = curl_slist_append(recipients, email.getBCC(i).c_str());
		}


		//Set the recipient list
		curl_easy_setopt(this->curl, CURLOPT_MAIL_RCPT, recipients);

		//Dump the contents of the email to a file in an effort to avoid callback functions etc
		//NOTE This would be much better to do via a callback function to send the payload text.
		//NOTE The tempfile functions in stdlib.h use a binary interface which would have some undefined effects on our payload text

		std::string payload = email.encode();
		std::ofstream ofs;
		ofs.open("tempEmail.txt", std::ofstream::out);
		ofs << payload;
		ofs.close();

		FILE * pfile;
		pfile = fopen("tempEmail.txt", "r");

		if(pfile == NULL) {
			throw std::runtime_error("Error connecting to SMTP server: Unable to create temporary email container");
		}

		curl_easy_setopt(this->curl, CURLOPT_READDATA, pfile);

		//Set status
		this->res = this->CONNECTION_OPEN;

		//Send the temporary file via CURL
		this->res = this->SENDING_DATA;
		CURLcode result = curl_easy_perform(this->curl);
		this->checkConnection(result);

		//Set status
		this->res = this->SENDING_COMPLETE;

		//Delete the recipients list
		curl_slist_free_all(recipients);

		//Delete the temporary file
		fclose(pfile);
		remove("tempEmail.txt");
		pfile = NULL;

		this->res = this->CONNECTION_OPEN;
	}
	else {
		throw std::runtime_error("Error connection to SMTP server: Attempt to send mail failed because of closed connection");
	}

}

int SMTPConnection::getStatus(){
	return this->res;
}

std::string SMTPConnection::getAddress(){
	return this->address;
}

std::string SMTPConnection::getUsername(){
	return this->username;
}

std::string SMTPConnection::getPassword(){
	return this->password;
}

void SMTPConnection::checkConnection(unsigned int toCheck){
	//Make sure the sending completed successfully
	if(toCheck != CURLE_OK){
		std::ostringstream oss;
		oss<<"Error connecting to SMTP server: CURL returned the error " <<toCheck;
		throw std::runtime_error(oss.str());
	}
}

} /* namespace SimplyEmail */

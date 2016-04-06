/**
 * \file Email.h
 *
 * \brief Header file for Email object.
 *
 * \author Neale Petrillo <neale.a.petrillo@gmail.com>
 * \copyright Neale Petrillo, 2015
 */

#ifndef EMAIL_H_
#define EMAIL_H_

#include <string>
#include <sstream>
#include <cwctype>
#include <vector>
#include <stdexcept>
#include <ctime>
#include <cstdlib>

#include "./EmailAttachment.h"

/**
 * \brief Container for external communication functionality.
 *
 * \details Contains and other objects to be used to communicate outside of the program.
 *
 */
namespace Communication {

/**
 * \brief Contains functions and members that generate an email for sending via SMTP.
 *
 * \details Contains functions and members to facilitate generating a MIME encoded email that can be went with
 * via SMTP.
 */
class Email {
public:
	/**
	 * \brief Default constructor
	 *
	 * \details Crates a blank email
	 *
	 * \return void
	 */
	Email();

	/**
	 * \brief Parametrized constructor
	 *
	 * \details Uses given parameters to create a mail message
	 *
	 * \param[in] recipient The address to send the message to
	 * \param[in] cc The address to CC the message to
	 * \param[in] bcc The address to BCC the message to
	 * \param[in] from The address to list as the sender
	 * \param[in] replyTo The address to list as the replyto address
	 * \param[in] subject The message subject
	 * \param[in] body The body text of the message
	 *
	 * \return void
	 */
	Email(std::string recipient, std::string cc, std::string bcc, std::string from, std::string replyTo, std::string subject, std::string body);

	/**
	 * \brief Parametrized constructor
	 *
	 * \details Uses given parameters to create a mail message to be sent to multiple recipients
	 *
	 * \param[in] recipient The list of address to send the message to
	 * \param[in] cc The address to CC the message to
	 * \param[in] bcc The address to BCC the message to
	 * \param[in] from The address to list as the sender
	 * \param[in] replyTo The address to list as the replyto address
	 * \param[in] subject The message subject
	 * \param[in] body The body text of the message
	 *
	 * \return void
	 */
	Email(std::vector<std::string> recipients,std::vector<std::string> cc, std::vector<std::string> bcc, std::string from, std::string replyTo, std::string subject, std::string body);

	//TODO Add other parametrized constructors to allow any combination of vector address inputs.

	/**
	 * \brief Copy constructor
	 *
	 * \details Copy constructor to facilitate standard containers
	 *
	 * \param[in] other Reference to previously instantiated email
	 *
	 * \return void
	 */
	Email(Email& other);

	/**
	 * \brief Default destructor
	 *
	 * \details Destroys an instance of email. Currently not implemented.
	 *
	 * \return void
	 */
	~Email();

	/**
	 * \brief Encodes email data for sending
	 *
	 * \details Takes saved email members and encodes them for sending via SMTP
	 *
	 * \return std::string The encoded email message
	 */
	std::string encode();

	const std::string getRecipient(unsigned int recipientNumber);
	const std::vector<std::string> getRecipients();
	unsigned int getRecipientNumber();
	void addRecipient(const std::string recipient);

	const std::string getCC(unsigned int ccNumber);
	const std::vector<std::string> getCCs();
	unsigned int getCCNumber();
	void addCC(const std::string& recipient);

	const std::string getBCC(unsigned int bccNumber);
	const std::vector<std::string> getBCCs();
	unsigned int getBCCNumber();
	void addBCC(const std::string& recipient);

	const std::string getBody() const;
	void setBody(const std::string& body);

	const std::string getFrom() const;
	void setFrom(const std::string& from);

	const std::string getReplyTo() const;
	void setReplyTo(const std::string& replyTo);

	const std::string getSubject() const;
	void setSubject(const std::string& subject);

	const Communication::EmailAttachment getAttachment(unsigned int attachmentNumber);
	const std::vector<Communication::EmailAttachment> getAttachments();
	unsigned int getAttachmentNumber();
	void addAttachment(const std::string& fileLocation);

private:
	std::vector<std::string> recipients;								/// List of recipient addresses. Must be confirmed to be syntactically correct to add to the list.
	std::vector<std::string> cc;										/// List of cc recipient addresses. Must be confirmed to be syntactically correct to add to the list.
	std::vector<std::string> bcc;										/// List of bcc recipient addresses. Must be confirmed to be syntactically correct to add to the list.
	std::string from;													/// The address to list as "from"
	std::string replyTo;												/// The address to list as "replyTo"
	std::string subject;												/// The text to be contained int he subject
	std::string body;													/// Text to be appended to the generated text of the email message

	std::vector<Communication::EmailAttachment> attachments;			/// List of attachments to be sent with the message

	static const std::string bodyType;									/// The MIME type of the body; currently only plain text is supported.
	static const std::string bodyCharSet;								/// The character set of the body text; currently only UTF-8 is supported.
	static const std::string boundryText;								/// The text to be used to encase boundries
	static const std::string endLineText;								/// The text to be used to end a line

	std::string encodeHeader();
	std::string encodeBody();
	std::string encodeAttachments();
	
	/**
	 * \brief Encodes a vector of strings in a comma seperated list
	 *
	 */
	std::string encodeVector(std::vector<std::string> toEncode);
	
	/**
	 * \brief Creates a timestamp in proper email format
	 *
	 * \details Gets the current time and generates a tiemstamp in the proper email format.
	 *
	 * \return std::string The current timestamp in email format
	 */
	std::string createTimestamp();

	/**
	 * \brief Checks a given string to test whether or not it is a valid email address
	 *
	 * \details Takes a string as a parameter and returns true if it is a valid email address or false if it is not.
	 * This function currently does not make use of regular expressions as they are not thoroughly implemented in g++
	 * 4.4.5 - the release currently utilized by several embedded systems. Future modification should implement regular
	 * expressions for more robust and faster operation.
	 *
	 * \param[in] addressToTest The string and potential email address to test
	 *
	 * \return bool True if the given string is an email address false otherwise.
	 */
	bool isAddress(const std::string& addressToTest);
};

} /* namespace DataAnalysis */

#endif /* EMAIL_H_ */

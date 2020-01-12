/**
 * \file EmailAttachment.h
 *
 * \brief Header file for email attachment object
 *
 * \details Header file for email attachment object to be used by Email object
 */

#ifndef EMAILATTACHMENT_H_
#define EMAILATTACHMENT_H_

#include <string>
#include <fstream>
#include <utility>
#include <stdexcept>

namespace SimplyEmail {

/**
 * \brief Container for email attachments
 *
 * \details Contains information about email attachments to be used to generate MIME type email attachments
 */
class EmailAttachment {
public:

	/**
	 * \breif Default constructor
	 *
	 * \details Creates an empty email attachment
	 *
	 * \return void
	 */
	EmailAttachment();

	/**
	 * \brief Parametrized constructor
	 *
	 * \details Generates an email attachment using a provided file path.
	 *
	 * \param[in] fileAddress The path of a file relative to the root to be encoded.
	 *
	 * \return void
	 */
	EmailAttachment(std::string fileAddress);

	/**
	 * \brief Copy constructor
	 *
	 * \details Generates an email attachment using a previously instantiated email attachment
	 *
	 * \param[in] other A previously instantiated email attachment
	 *
	 * \return void
	 */
	EmailAttachment(const EmailAttachment& other);

	/**
	 * \brief Default destructor
	 *
	 * \details Destroys an attachment object
	 */
	~EmailAttachment();

	//TODO Document getters and setters
	const std::string getData() const;
	const std::string getFileName() const;
	const std::string getMimeType() const;

private:
	std::string mimeType;								/// The MIME type of the attachment
	std::string fileName;								/// The name of the attachment
	std::string data;									/// The encoded data of the attachment

	/**
	 * \brief Finds MIME type based on file extension
	 *
	 * \details Examines a file looking for the file's extension, determines a MIME type based on an internal list, .
	 * The file path may be incomplete; however, the string must include a '.' preceding the extension.
	 * The internal extension list is incomplete however it includes most of the commonly generated report types.
	 *
	 * \param[in] filePath The path to the file whose MIME type is to be determined. May be the file's name but must include '.' preceding the extension.
	 *
	 * \return void
	 */
	void findMIMEType(std::string filePath);

	/**
	 * \brief Encodes a file into base 64
	 *
	 * \details Takes a path to a file and encodes it into a base 64 string then sets the internal data member.
	 * The file path must be the full path to the file and the file must be readable.
	 *
	 * \param[in] filePath The full path to the file to be encoded.
	 *
	 * \return void
	 */
	void encodeFile(std::string filePath);

	/**
	 * \brief Encodes three bytes represented as a std::string into four bytes represented as an std::string
	 *
	 * \details Takes a three character string representing three bytes and converts it into a four character string
	 * representing the base 64 conversion.
	 *
	 * \param[in] toEncode A three character string to be encoded
	 *
	 * \return std::string A four character string containing the base 64 encoding of the input string
	 */
	std::string encodeBytes(std::string toEncode);

	/**
	 * \brief Encodes three bytes represented as a char array into four bytes represented as an std::string
	 *
	 * \details Takes a three character string representing three bytes and converts it into a four character string
	 * representing the base 64 conversion.
	 *
	 * \param[in] toEncode A three character string to be encoded
	 *
	 * \return std::string A four character string containing the base 64 encoding of the input string
	 */
	std::string encodeBytes(char* toEncode);


};

} /* namespace SimplyEmail */

#endif /* EMAILATTACHMENT_H_ */

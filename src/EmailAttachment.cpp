/**
 * \file EmailAttachment.cpp
 *
 * \brief Implementation file for Email Attachment object
 *
 * \author Neale Petrillo <neale.a.petrillo@gmail.com>
 * \copyright Neale Petrillo, 2015
 */
#include "../lib/EmailAttachment.h"

namespace Communication {

EmailAttachment::EmailAttachment() {
	this->mimeType = "";
	this->fileName = "";
	this->data = "";
}

EmailAttachment::EmailAttachment(std::string fileAddress){
	// Try to encode the file
	try {
		this->encodeFile(fileAddress);
	}
	catch (const std::runtime_error& e){
		throw;
	}

	// Try to detect mime type
	try  {
		this->findMIMEType(fileAddress);
	}
	catch (const std::runtime_error& e){
		throw;
	}

	// Get the name of the file from the address
	int i=fileAddress.length() - 1;
	while((i > -1) && (fileAddress.compare(i,1,"/"))){
		i = i-1;
	}

	this->fileName = fileAddress.substr((unsigned)(i+1),std::string::npos);
}

EmailAttachment::EmailAttachment(const EmailAttachment& other){
	this->mimeType = other.getMimeType();
	this->fileName = other.getFileName();
	this->data = other.getData();
}

EmailAttachment::~EmailAttachment() {
	// Nothing to destroy :(
}

const std::string EmailAttachment::getData() const {
	return data;
}

const std::string EmailAttachment::getFileName() const {
	return fileName;
}

const std::string EmailAttachment::getMimeType() const {
	return mimeType;
}

void EmailAttachment::findMIMEType(std::string filePath){
	/* Create a list of extensions and their MIME types
	 * The list is incomplete but incorporates the most likely files for our types
	 * Types[0] is considered the default for files without an extension or an unknown type
	 */

	//NOTE The type array could potentially be an std::iterator to make the number of types variable unnecessary but including more than the utilities library seems like overkill
	unsigned int numberOfTypes = 15;
	std::pair <std::string, std::string> types [15];

	types[0] = std::make_pair("", "application/unknown");
	types[1] = std::make_pair("bin", "application/octet-stream");
	types[2] = std::make_pair("pdf", "application/pdf");
	types[3] = std::make_pair("xhtml", "application/xhtml+xml");
	types[4] = std::make_pair("xht", "application/xhtml+xml");
	types[5] = std::make_pair("zip", "application/zip");
	types[6] = std::make_pair("gzip","application/gzip");
	types[7] = std::make_pair("gif", "image/gif");
	types[8] = std::make_pair("jpg", "images/jpeg");
	types[9] = std::make_pair("jpeg", "images/jpeg");
	types[10] = std::make_pair("png", "images/png");
	types[11] = std::make_pair("csv", "text/csv");
	types[12] = std::make_pair("html", "text/html");
	types[13] = std::make_pair("txt", "text/plain");
	types[14] = std::make_pair("xml", "text/xml");


	//Get file extension
	//Back up to the first '.'
	unsigned int i = (filePath.length() - 1);
	while((filePath[i] != '.') && (i > 0)) {
		i = i -1;
	}

	//Check to make sure we found a .
	std::string fileExtension = "";

	if( i > 0 ) {
		fileExtension = filePath.substr(i+1);
	}

	//Clear the stored MIME type
	this->mimeType = "";

	i = 0;
	while((i < numberOfTypes) && (this->mimeType.compare("") == 0)){
		if(fileExtension.compare(types[i].first) == 0){
			this->mimeType = types[i].second;
		}

		i = i+1;
	}

	//If we didn't find an extension, set it to the default
	if(this->mimeType.compare("") == 0){
		this->mimeType = types[0].second;
	}

}

void EmailAttachment::encodeFile(std::string filePath) {

	//Create return variable
	std::string toReturn;

	//Open the file
	std::ifstream inputStream;
	inputStream.open(filePath.c_str(), std::fstream::binary);

	//NOTE The following section relies on the internal file stream pointer. It may be more reliable to use seekg and an absolute position.
	if(inputStream.is_open()){

		//Calculate the size of the file
		inputStream.seekg(0, inputStream.end);
		int fileLength = inputStream.tellg();

		//Return to the beginning of the file
		inputStream.seekg(inputStream.beg);

		//Read all the groups of 3
		for(int i=0; i < (int)(fileLength/3); i++){
			char* tempChars = new char [3];
			inputStream.read(tempChars,3);

			toReturn = toReturn + encodeBytes(tempChars);
		}

		//Read last remaining 1 or 2 and pad with zeros
		int oddCharacters = fileLength % 3;
		if(oddCharacters > 0) {

			char* tempChars = new char [3];
			inputStream.read(tempChars, oddCharacters);

			for(int i= oddCharacters; i<3; i++){
				tempChars[i] = '0';
			}

			//Convert the padded string then remove unnecesary padding
			toReturn = toReturn + encodeBytes(tempChars).substr(0,(oddCharacters + 1));

			char padChar = '=';
			for(int i = 0; i<(3-oddCharacters); i++){
				toReturn = toReturn + padChar;
			}
		}

		//Close file
		inputStream.close();

		//Save the results
		this->data = toReturn;
	}
	else {
		throw std::runtime_error("Error creating attachment: could not open file.");
	}

}

std::string EmailAttachment::encodeBytes(char* toEncode) {
	std::string tempString(toEncode, 3);

	return encodeBytes((std::string)tempString);

}

std::string EmailAttachment::encodeBytes(std::string toEncode){

	//Create return variable
	std::string toReturn (4, ' '); // Return string must be four characters long for proper encoding

	if(toEncode.length() == 3) {
		//Build encoder table
		static const std::string baseChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

		//Combine the characters to form a number
		int encodedValue[4] = {0,0,0,0};

		encodedValue[0] = ((toEncode[0] & 0xfc) >> 2);
		encodedValue[1] = ((toEncode[0] & 0x03) << 4) + ((toEncode[1] & 0xf0) >> 4);
		encodedValue[2] = ((toEncode[1] & 0x0f) << 2) + ((toEncode[2] & 0xc0) >> 6);
		encodedValue[3] = (toEncode[2] & 0x3f);

		//Lookup corresponding values and add them to the return variable
		for(unsigned int i = 0; i<4; i++){
			toReturn[i] = baseChars[encodedValue[i]];
		}

	}
	else {
		throw std::runtime_error("Error creating attachment: could not encode byte.");
	}

	return toReturn;
}
} /* namespace Communication */

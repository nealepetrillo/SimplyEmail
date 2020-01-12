/**
 * \file Email.cpp
 *
 * \brief Implementation file for Email object
 *
 * \author Neale Petrillo <neale.a.petrillo@gmail.com>
 * \copyright Neale Petrillo, 2015
 */

#include "../lib/Email.h"

namespace SimplyEmail {

const std::string Email::bodyType = "text/plain";
const std::string Email::bodyCharSet = "UTF-8";
const std::string Email::boundryText = "gc0p4Jq0M2Yt08jU534c0p";
const std::string Email::endLineText = "\r\n";

Email::Email() {

	this->recipients = std::vector<std::string> (0);
	this->cc = std::vector<std::string> (0);
	this->bcc = std::vector<std::string> (0);
	this->from = "";
	this->replyTo = "";
	this->subject = "";
	this->body = "";
	this->attachments = std::vector<SimplyEmail::EmailAttachment> (0);

}

Email::Email(std::string recipient, std::string _cc, std::string _bcc, std::string _from, std::string _replyTo, std::string _subject, const std::string &_body){
	this->recipients = std::vector<std::string> (0);
	this->addRecipient(recipient);

	this->cc = std::vector<std::string> (0);
	this->addCC(_cc);

	this->bcc = std::vector<std::string> (0);
	this->addBCC(_bcc);

	this->from = _from;
	this->replyTo = _replyTo;
	this->subject = _subject;
	this->body = _body;

	this->attachments = std::vector<SimplyEmail::EmailAttachment> (0);
}


Email::Email(std::vector<std::string> _recipients,std::vector<std::string> _cc, std::vector<std::string> _bcc, std::string _from, std::string _replyTo, std::string _subject, const std::string &_body){
	this->recipients = _recipients;
	this->cc = _cc;
	this->bcc = _bcc;
	this->from = _from;
	this->replyTo = _replyTo;
	this->subject = _subject;
	this->body = _body;

	this->attachments = std::vector<SimplyEmail::EmailAttachment> (0);
}


Email::Email(Email& other){

	this->recipients = other.getRecipients();
	this->cc = other.getCCs();
	this->bcc = other.getBCCs();
	this->from = other.getFrom();
	this->replyTo = other.getReplyTo();
	this->subject = other.getSubject();
	this->body = other.getBody();

	this->attachments = other.getAttachments();
}

Email::~Email() {
	// Nothing to destroy :(
}

std::string Email::encode() {

	//Check to make sure recipients are listed
	if(this->recipients.size() < 1){
		throw std::runtime_error("Error generating email: no recipients listed");
	}

	std::stringstream stream;

	stream 	<< this->encodeHeader()
			<< "--" << this->boundryText
			<< this->endLineText;

	stream  << this->encodeBody();

	if(this->getAttachmentNumber() > 0) {
		stream 	<< this->encodeAttachments()
				<< this->endLineText
				<< "--" << this->boundryText << "--";
	}

	return stream.str();
}

const std::string Email::getRecipient(unsigned int recipientNumber) {
	if(recipientNumber > this->recipients.size()){
		throw std::out_of_range("Error getting email recipient: recipient number out of range");
	}
	else {
		return this->recipients[recipientNumber];
	}

}

const std::vector<std::string> Email::getRecipients(){
	return this->recipients;
}

unsigned int Email::getRecipientNumber(){
	return this->recipients.size();
}

void Email::addRecipient(const std::string recipient){

	if(this->isAddress(recipient)){
		this->recipients.push_back(recipient);
	}
	else {
		throw std::runtime_error("Error adding recipient: invalid email address.");
	}
}

const std::string Email::getCC(unsigned int ccNumber){
	if(ccNumber > this->cc.size()){
		throw std::out_of_range("Error getting email cc: cc number out of range");
	}
	else{
		return this->cc[ccNumber];
	}

}

const std::vector<std::string>Email::getCCs(){
	return this->cc;
}

unsigned int Email::getCCNumber(){
	return this->cc.size();
}

void Email::addCC(const std::string& recipient){
	if(this->isAddress(recipient)){
		this->cc.push_back(recipient);
	}
	else {
		throw std::runtime_error("Error adding cc: invalid email address.");
	}
}

const std::string Email::getBCC(unsigned int bccNumber){
	if(bccNumber > this->bcc.size()){
		throw std::out_of_range("Error getting bcc: bcc number out of range");
	}
	else {
		return this->bcc[bccNumber];
	}
}

const std::vector<std::string> Email::getBCCs(){
	return this->bcc;
}

unsigned int Email::getBCCNumber(){
	return this->bcc.size();
}

void Email::addBCC(const std::string& recipient){
	if(this->isAddress(recipient)){
		this->bcc.push_back(recipient);
	}
	else {
		throw std::runtime_error("Error adding bcc: invalid email address.");
	}
}

const std::string Email::getBody() const {
	return body;
}

void Email::setBody(const std::string& _body) {
	this->body = _body;
}

const std::string Email::getFrom() const {
	return from;
}

void Email::setFrom(const std::string& _from) {
	this->from = _from;
}

const std::string Email::getReplyTo() const {
	return replyTo;
}

void Email::setReplyTo(const std::string& _replyTo) {
	this->replyTo = _replyTo;
}

const std::string Email::getSubject() const {
	return subject;
}

void Email::setSubject(const std::string& _subject) {
	this->subject = _subject;
}

const SimplyEmail::EmailAttachment Email::getAttachment(unsigned int attachmentNumber){
	if(attachmentNumber > this->attachments.size()){
		throw std::out_of_range("Error getting attachment: attachment number out of range");
	}
	else {
		return this->attachments[attachmentNumber];
	}
}

const std::vector<SimplyEmail::EmailAttachment> Email::getAttachments(){
	return this->attachments;
}

unsigned int Email::getAttachmentNumber(){
	return this->attachments.size();
}

void Email::addAttachment(const std::string& fileLocation){

	try{
		SimplyEmail::EmailAttachment tempAttachment(fileLocation);

		this->attachments.push_back(tempAttachment);
	}
	catch(std::runtime_error& e) {
		throw;
	}
}

std::string Email::encodeHeader(){
	//NOTE The format for this message was taken from sample GMail messages. The order may not matter but best to do it like a large, multinational, technology firm.
	std::stringstream stream;

	//Add from
	stream 	<< "From: " << "<" << this->from << ">" << this->endLineText;

	//Add to
	stream << "To: " << this->encodeVector(this->recipients) << this->endLineText;

	//Add cc
	if (this->getCCNumber() > 0) {
		stream << "Cc: " << this->encodeVector(this->cc) << this->endLineText;
	}

	//Add BCC
	if(this->getBCCNumber() > 0) {
		stream << "Bcc: " << this->encodeVector(this->bcc) << this->endLineText;
	}

	//Add subject
	stream 	<< "Subject: " << this->subject	<< this->endLineText;

	//Add date
	stream 	<< this->createTimestamp() << this->endLineText;

	//Add MIME Line
	stream	<< "MIME-Version: 1.0"<<this->endLineText;

	//Add content type
	if(this->getAttachmentNumber() > 0) {
		stream 	<< "Content-Type: multipart/mixed; ";
	}
	else {
		stream 	<< "Content-Type: text/plain; ";
	}

	stream << "boundary=\"" << this->boundryText << "\"" << this->endLineText;

	return stream.str();
}

std::string Email::encodeBody(){
	std::stringstream stream;

	//Add content type
	stream	<< "Content-Type: " << this->bodyType << "; charset=" << this->bodyCharSet 	<< this->endLineText<< this->endLineText;

	//Add body text
	stream 	<< this->body << this->endLineText;

	return stream.str();
}

std::string Email::encodeAttachments(){

	//Create constants and return variables for ID generation
	static const std::string lookup = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	std::stringstream stream;

	//For each attachment add a boundry and its data
	for(unsigned int i=0; i<this->getAttachmentNumber(); i++){

		//Add the boundry line
		stream 	<< "--" << this->boundryText << this->endLineText;

		//Add the content type
		stream 	<< "Content-Type: " << this->getAttachment(i).getMimeType() << "; name=\""
				<< this->getAttachment(i).getFileName() << "\"" << this->endLineText;

		//Add content disposition
		stream 	<< "Content-Disposition: attachment; filename=\""
				<< this->getAttachment(i).getFileName()
				<< "\""
				<< this->endLineText;

		//Add encoding informatione
		stream 	<< "Content-Transfer-Encoding: base64"
				<< this->endLineText;

		//Add attachment id
		stream	<< "X-Attachment-Id: ";

		for(int j=0; j<11; j++){
			stream << lookup[rand() % (lookup.length()-1)];
		}

		stream 	<< this->endLineText
				<< this->endLineText;

		//Add the data
		stream	<< this->getAttachment(i).getData()
				<< this->endLineText << this->endLineText;

	}

	return stream.str();
}

std::string Email::createTimestamp(){
	std::stringstream stream;

	std::time_t generationTime;
	struct std::tm * timeInfo;
	time(&generationTime);
	timeInfo = gmtime(&generationTime);

	std::string dayNames[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
	std::string monNames[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

	int timeZone = timeInfo->tm_hour;

	stream 	<< "Date: "
			<< dayNames[timeInfo->tm_wday]
			<< ", "
			<< timeInfo->tm_mday
			<< ' '
			<< monNames[timeInfo->tm_mon]
			<< ' '
			<< (timeInfo->tm_year + 1900)
			<< ' '
			<< (timeInfo->tm_hour)
			<< ':'
			<< (timeInfo->tm_min)
			<< ':'
			<< (timeInfo->tm_sec)
			<< ' ';

	timeInfo = localtime(&generationTime);
	timeZone = ((timeZone - timeInfo->tm_hour) * 100);

	if(timeZone > 0) {
		stream << '+';
	}
	else
	{
		stream << '-';
	}

	stream	<< timeZone;

	return stream.str();
}

std::string Email::encodeVector(std::vector<std::string> toEncode){
	std::stringstream stream;

	for(unsigned int i=0; i<toEncode.size(); i++){

		stream << "<";

		if( i > 0){
			stream << ",";
		}

		stream<<toEncode[i];

		stream << ">";
	}

	return stream.str();
}

bool Email::isAddress(const std::string& addressToTest){

	//Create a return variable
	bool toReturn = true;

	//If it's an email address it must have no less than [a-z]@[a-z].[a-z] characters
	if(addressToTest.length() > 5) {
		//Split the string at the @ symbol
		std::size_t location = addressToTest.find('@');

		if(location == std::string::npos){
			//The address does not contain the @ symbol. It isn't valid
			toReturn = false;
		}
		else {
			std::string beforeAt = addressToTest.substr(0,location);
			std::string afterAt = addressToTest.substr(location+1);

			//Check to make sure there's a period in the last half of the string
			location = afterAt.find('.');

			if(location == std::string::npos){
				toReturn = false;
			}
			else {
				/*
				 * Since there are a lot of top level domains, just check to make sure
				 * the length following the . is reasonable
				 */
				if(((afterAt.length() - location) < 3) || ((afterAt.length() - location) > 63)){
					toReturn = false;
				}
				else {
					//Check for whitespaces in first half of string
					unsigned int i = 0;
					while((i < beforeAt.length()) && (toReturn == true)){

						if((bool)iswspace(beforeAt.at(i)) == true) {
							toReturn = false;
						}
						i++;
					}

					//Check for whitespaces in last half of string
					i = 0;
					while((i < afterAt.length()) && (toReturn == true)){

						if((bool)iswspace(afterAt.at(i)) == true) {
							toReturn = false;
						}
						i++;
					}
				}
			}
		}
	}
	else {
		toReturn = false;
	}

	return toReturn;
}
} /* namespace SimplyEmail */

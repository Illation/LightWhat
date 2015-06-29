//XML Parser by Robert Lindner - 1DAE3

#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "XMLdom.hpp"

struct XMLelemnt;
struct XMLattribute;
enum XMLtokenType{
	XML_SMALLER,
	XML_GREATER,
	XML_QUESTION,
	XML_EXCLAMATION,
	XML_EQUALS,
	XML_SLASH,
	XML_QUOTE,
	XML_AMPERSAND,
	XML_APOSTROPHE,
	XML_WHITESPACE,
	XML_TEXT
};
struct XMLtoken{

	XMLtokenType type;
	char content;
};

class XMLParser
{
public:
	XMLParser();
	~XMLParser();

	void loadDocument(const char *filename);
	void parse(XMLdom *dom);

	std::vector<XMLtoken> tokens;
private:
	XMLtokenType getTokenType(char c);
	std::wstring getTextContent(size_t &posRef);
	std::vector<XMLattribute> getAttributes(size_t &posRef);
	XMLattribute getAttribute(size_t &posRef);
	XMLelement *getElement(size_t &posRef);
};


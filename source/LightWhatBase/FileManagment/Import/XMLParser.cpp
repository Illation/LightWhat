#include "XMLParser.hpp"


XMLParser::XMLParser()
{
}


XMLParser::~XMLParser()
{
}

void XMLParser::loadDocument(const char *filename)
{
	using namespace std;
	tokens.clear();
	wifstream xmlFile(filename);
	while (xmlFile.eof() == false)
	{
		wstring extractedLine;
		getline(xmlFile, extractedLine);
		while (extractedLine.size()>0)
		{
			XMLtoken thisToken;
			thisToken.type = getTokenType(extractedLine.at(0));
			thisToken.content = extractedLine.at(0);
			int it = 1;
			tokens.push_back(thisToken);
			extractedLine = extractedLine.substr(it, extractedLine.size());
		}
	}
}

void XMLParser::parse(XMLdom *dom)
{
	using namespace std;
	for (size_t docPos = 0; docPos < tokens.size(); docPos++)
	{
		if (tokens[docPos].type == XML_SMALLER)
		{
			if (tokens[docPos + 1].type == XML_QUESTION)//XML Header
			{
				while (!(tokens[docPos].type == XML_TEXT))
				{
					docPos++;
				}
				wstring xml = getTextContent(docPos);
				dom->attributes = getAttributes(docPos);
			}
			else//XML root
			{
				dom->root = getElement(docPos);
			}
		}
	}
}

XMLtokenType XMLParser::getTokenType(char c)
{
	XMLtokenType ret = XML_TEXT;
	if (c == '<')ret = XML_SMALLER;
	if (c == '>')ret = XML_GREATER;
	if (c == '\?')ret = XML_QUESTION;
	if (c == '!')ret = XML_EXCLAMATION;
	if (c == '=')ret = XML_EQUALS;
	if (c == '/')ret = XML_SLASH;
	if (c == '\"')ret = XML_QUOTE;
	if (c == '&')ret = XML_AMPERSAND;
	if (c == '\'')ret = XML_APOSTROPHE;
	if (c == ' ')ret = XML_WHITESPACE;
	if (c == '\t')ret = XML_WHITESPACE;
	return ret;
}
std::wstring XMLParser::getTextContent(size_t &posRef)
{
	using namespace std;
	wstring ret;
	while (true)
	{
		if (tokens[posRef].type == XML_TEXT)
		{
			ret += tokens[posRef].content;
			posRef++;
		}
		else
		{
			return ret;
		}
	}
}

std::vector<XMLattribute> XMLParser::getAttributes(size_t &posRef)
{
	using namespace std;
	vector<XMLattribute> ret;
	while ((!(tokens[posRef].type == XML_SLASH)) && (!(tokens[posRef].type == XML_GREATER)) && (!(tokens[posRef].type == XML_QUESTION)))
	{
		if (tokens[posRef].type == XML_TEXT)
		{
			ret.push_back(getAttribute(posRef));
		}
		else
		{
			posRef++;
		}
	}
	return ret;
}

XMLattribute XMLParser::getAttribute(size_t &posRef)
{
	using namespace std;
	XMLattribute ret;
	while (!(tokens[posRef].type == XML_TEXT))posRef++;
	ret.name = getTextContent(posRef);
	while (!(tokens[posRef].type == XML_EQUALS))posRef++;
	while (!(tokens[posRef].type == XML_QUOTE))posRef++;
	posRef++;
	while (!(tokens[posRef].type == XML_QUOTE))
	{
		ret.content += tokens[posRef].content;
		posRef++;
	}
	return ret;
}

XMLelement *XMLParser::getElement(size_t &posRef)
{
	using namespace std;
	XMLelement *ret = new XMLelement;
	ret->isLeaf = false;
	while (!(tokens[posRef].type == XML_TEXT))
	{
		posRef++;
	}
	ret->name = getTextContent(posRef);
	ret->attributes = getAttributes(posRef);
	if (tokens[posRef].type == XML_SLASH)
	{
		while (!(tokens[posRef].type == XML_GREATER))posRef++;
		posRef++;
		return ret;
	}
	if (tokens[posRef].type == XML_GREATER)
	{
		posRef++;
		bool elementParsed = false;
		while (elementParsed == false)
		{
			if (tokens[posRef].type == XML_SMALLER)
			{
				if (tokens[posRef + 1].type == XML_SLASH)//escape the node
				{
					elementParsed = true;
					posRef += 2;
					wstring retName = getTextContent(posRef);
					if (!(retName == ret->name))
					{
						wcout << "wrong element return name, expected: " << ret->name << " , found: " << retName << endl;
					}
					while (!(tokens[posRef].type == XML_GREATER))posRef++;
					posRef++;
					return ret;
				}
				else//subnode
				{
					ret->children.push_back(getElement(posRef));
				}
			}
			else//content
			{
				ret->content += tokens[posRef].content;
				posRef++;
			}
				
			
		}
	}
	return ret;
}
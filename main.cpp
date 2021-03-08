#include <iostream>
#include <future>
#include "tinyxml2.h"

int example_3(){
    static const char* xml =
            "<?xml version=\"1.0\"?>"
            "<!DOCTYPE PLAY SYSTEM \"play.dtd\">"
            "<PLAY>"
            "<TITLE>A Midsummer Night's Dream</TITLE>"
            "</PLAY>";

    tinyxml2::XMLDocument doc;
    doc.Parse( xml );

    tinyxml2::XMLElement* titleElement = doc.FirstChildElement( "PLAY" )->FirstChildElement( "TITLE" );
    const char* title = titleElement->GetText();
    printf( "Name of play (1): %s\n", title );

    tinyxml2::XMLText* textNode = titleElement->FirstChild()->ToText();
    title = textNode->Value();
    printf( "Name of play (2): %s\n", title );

    return doc.ErrorID();
}

bool exampler_4(){
    static const char* xml =
		"<information>"
		"	<attributeApproach v='2' />"
		"	<textApproach>"
		"		<v>2</v>"
		"	</textApproach>"
		"</information>";

    tinyxml2::XMLDocument doc;
	doc.Parse( xml );

	int v0 = 0;
	int v1 = 0;

    tinyxml2::XMLElement* attributeApproachElement = doc.FirstChildElement()->FirstChildElement( "attributeApproach" );
	attributeApproachElement->QueryIntAttribute( "v", &v0 );

    tinyxml2::XMLElement* textApproachElement = doc.FirstChildElement()->FirstChildElement( "textApproach" );
	textApproachElement->FirstChildElement( "v" )->QueryIntText( &v1 );

	printf( "Both values are the same: %d and %d\n", v0, v1 );

	return !doc.Error() && ( v0 == v1 );
}

void tread_check(tinyxml2::XMLElement * root, std::promise<std::string> &result){

    //std::string str("<root></root>");
    //result.set_value(str);

    result.set_value("<root></root>");
}


int test3() {
    std::cout << "Hello, World!" << std::endl;

   // example_3();
   // exampler_4();

    //std::string xml = "<?xml version='1.0' encoding='utf-8'?><root><body><items><item>1</item><item></item><item>3</item></items></body></root>";
    std::string xml = "<?xml version='1.0' encoding='utf-8'?><root><head><transcode></transcode></head><body><items></items></body></root>";
    //std::string xml = "<?xml version='1.0' encoding='utf-8'?><root><body><items>123456</items></body></root>";

    tinyxml2::XMLDocument doc;

    if (tinyxml2::XML_SUCCESS != doc.Parse(xml.c_str(),xml.length())){

        std::cout  << "XML Parse failed." << std::endl;
        doc.Clear();
        return 1;
    }

    tinyxml2::XMLElement *root = doc.FirstChildElement("root");

    std::promise<std::string> result;
    auto f1 = result.get_future();
    //std::future<std::string&> ff = result.get_future();

    std::thread t1(tread_check, root, std::ref(result));

    t1.join();

    std::cout << f1.get() << std::endl;

/*
    tinyxml2::XMLNode* root =  doc.FirstChild();
    tinyxml2::XMLNode* body = root->FirstChild();
    tinyxml2::XMLNode* items = body->FirstChild();
*/
#if 1

    tinyxml2::XMLElement *transcode = root->FirstChildElement("head")->FirstChildElement("transcode");
    tinyxml2::XMLElement *body = root->FirstChildElement("body");
    tinyxml2::XMLElement *items = body->FirstChildElement("items");
#endif

    if (items){
        std::cout << "Found items node." << std::endl;
        //std::cout << items->NoChildren() << std::endl;
        //tinyxml2::XMLNode* node = items->FirstChild();
        tinyxml2::XMLNode* node = items->FirstChild();

        if (!node || node->NoChildren())
        {
            std::cout << "没子节点" << std::endl;

            const char *p = items->GetText();
            //const char *p = items->Value();
            //const char *p = NULL;

            std::cout << "-------------------------" << std::endl;

            char buffer[1024 + 1];
            // strncpy(buffer, 0, 1024);
            //sprintf(buffer,"%s", p);

            std::cout << "-------------------------" << std::endl;
        }
        else
        {
            std::cout << "有子节点" << std::endl;
        }
    }
    else
    {
        std::cout << "Not Found items node." << std::endl;
    }

    doc.Clear();
    return 0;
}

void create_test(){
    tinyxml2::XMLDocument *doc = new tinyxml2::XMLDocument();
    tinyxml2::XMLNode* element = doc->InsertEndChild(doc->NewElement("element"));

    tinyxml2::XMLElement *sub[3] = {doc->NewElement("sub"), doc->NewElement("sub"), doc->NewElement("sub")};
    for (int i =0; i <3;++i){
        sub[i]->SetAttribute("attrib", i);
    }
    element->InsertEndChild(sub[2]);

    const int dummyInitialValue = 1000;
    int dummyValue = dummyInitialValue;
    tinyxml2::XMLNode *comment = element->InsertFirstChild(doc->NewComment("comment"));
    comment->SetUserData(&dummyValue);
    element->InsertAfterChild(comment, sub[0]);
    element->InsertAfterChild(sub[0], sub[1]);
    sub[2]->InsertFirstChild(doc->NewText("& Text!"));
    doc->Print();

    element->DeleteChild(sub[2]);
    doc->DeleteNode(comment);

    element->FirstChildElement()->SetAttribute("attrib", true);
    element->LastChildElement()->DeleteAttribute("attribe");

    tinyxml2::XMLPrinter printer(0, true);
    doc->Print(&printer);
    std::cout << printer.CStr() << std::endl;


    //tinyxml2::XMLPrinter(0, true);


    doc->Print();


    delete doc;



}

int main(){

    create_test();

    return 0;

    std::string xml("<?xml version=\"1.0\" encoding=\"utf-8\"?><!--root-->");

    tinyxml2::XMLDocument doc;

    doc.Parse(xml.c_str());

    std::cout << doc.ErrorID() << std::endl;

    tinyxml2::XMLElement *root = doc.FirstChildElement();
    std::cout << doc.ErrorID() << std::endl;

    if (root){
        std::cout << "Find root\n";
    }

    return 0;
}
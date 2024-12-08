   #include <iostream>
   #include <bsoncxx/json.hpp>
   #include <bsoncxx/builder/stream/document.hpp>
   #include <bsoncxx/builder/stream/helpers.hpp>

   using bsoncxx::builder::stream::close_array;
   using bsoncxx::builder::stream::close_document;
   using bsoncxx::builder::stream::document;
   using bsoncxx::builder::stream::finalize;
   using bsoncxx::builder::stream::open_array;
   using bsoncxx::builder::stream::open_document;

   int main() {
       auto doc = document{} << "name" << "MyApplication"
                             << "version" << 1
                             << "features" << open_document
                             << "awesome" << true << close_document << finalize;

       bsoncxx::document::view view = doc.view();

       std::string name = view["name"].get_utf8().value.to_string();
       int version = view["version"].get_int32();

       auto features = view["features"].get_document().value;
       bool awesome = features["awesome"].get_bool();

       std::cout << "Name: " << name << "\nVersion: " << version 
                 << "\nAwesome: " << std::boolalpha << awesome << std::endl;

       return 0;
   }
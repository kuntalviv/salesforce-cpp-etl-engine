#include <napi.h>
#include <algorithm>
#include <cctype>

//Callback function that will run
Napi::Array processData(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::Array input = info[0].As<Napi::Array>();
  std::string method = info[1].As<Napi::String>().Utf8Value(); 
  uint32_t inputLength = input.Length();
  Napi::Array output = Napi::Array::New(env, inputLength);

  for(uint32_t i = 0; i < inputLength; i++) {
    Napi::Object record = input.Get(i).As<Napi::Object>();
    Napi::Object transformedRecord = Napi::Object::New(env);
    transformedRecord.Set("Id", record.Get("Id"));
    std::string name {record.Get("Name").As<Napi::String>().Utf8Value()};
    if(method == "toUpperCase") {
      
      std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) {return std::toupper(c);});
    } else if(method == "toLowerCase") {

      std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) {return std::tolower(c);});
    }
    transformedRecord.Set("Name", Napi::String::New(env, name));
    output.Set(i, transformedRecord);
  }

  return output;
}

//Making the callback available for js
Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "processData"), Napi::Function::New(env, processData));
  return exports;
}

//Entry point for node
NODE_API_MODULE(process_data, Init);

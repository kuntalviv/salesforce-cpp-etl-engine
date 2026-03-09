#include <napi.h>
#include <string>

std::string encryptData(const std::string& data) {
  std::string result {"Encrypted Data"};
  return result;
}

//Callback function that will run
Napi::Array processData(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::Array input = info[0].As<Napi::Array>();
  
  uint32_t inputLength = input.Length();
  Napi::Array output = Napi::Array::New(env, inputLength);

  std::string encryptedData {};
  for(uint32_t i = 0; i < inputLength; i++) {
    Napi::Object record = input.Get(i).As<Napi::Object>();
    std::string confidentialData = record.Get("PAN").As<Napi::String>().Utf8Value();
    encryptedData = encryptData(confidentialData);
    Napi::Object encryptedRecord = Napi::Object::New(env);
    encryptedRecord.Set("Id", record.Get("id"));
    encryptedRecord.Set("Encrypted PAN", Napi::String::New(env, encryptedData));
    output.Set(i, encryptedRecord);
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

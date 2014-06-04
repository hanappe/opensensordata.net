#include "datastream.h"

#include "db.h"

Handle<Array> Datastream::toV8Array(Isolate * i) {
  
  const std::vector<Datastream::Datarow*>& datarows = this->datarows;
  
  Handle<Array> parent_array = Array::New(i, datarows.size());
  
  
  int datarows_count = 0;
  for (std::vector<Datastream::Datarow*>::const_iterator it = datarows.begin(); it != datarows.end(); ++it, datarows_count++) {
    const Datastream::Datarow * dr = (*it);
    
    Handle<Array> rows_array = Array::New(i, dr->size());
    parent_array->Set(datarows_count, rows_array);
    
    int datapoints_count = 0;
    for (std::vector<Pair>::const_iterator itb = dr->begin(); itb != dr->end(); ++itb, datapoints_count++) {
      const Pair& p = (*itb);
      
      Handle<Array> pair_array = Array::New(i, 2);
      pair_array->Set(0, Integer::New(i, p.time));
      pair_array->Set(1, Number::New(i, p.value));
      
      rows_array->Set(datapoints_count, pair_array);
    }                                                                                                                                                                                
  }
  
  return parent_array;
    
}

void Datastream::JS::Constructor(const FunctionCallbackInfo<Value>& info) {
  v8::Isolate* i = v8::Isolate::GetCurrent();
  
  Handle<ObjectTemplate> ds_template = ObjectTemplate::New();
  ds_template->SetInternalFieldCount(1);


  const Handle<Value>& arg1 = info[0]; 

  const int datastream_id = !arg1.IsEmpty() && arg1->IsNumber() ? arg1->Int32Value() : 0;

  Datastream * datastream = DB::GetInstance()->getDatastream(datastream_id);
    
  if (datastream) {
    
    datastream->print();
			  

    // First way : call internaly the function "GetDatapoints" (and create a javascript array) each time "datapoints" is called in javascript
    ds_template->SetAccessor(String::NewFromUtf8(i, "datapoints"), GetDatapoints);

    // Second Way : create the datapoints one time and store it into javascript Datastream object.
    Handle<Array> array = datastream->toV8Array(i);
    ds_template->Set(String::NewFromUtf8(i, "datapointsB"), array);

    // set a javascript function WIP
    ds_template->Set(String::NewFromUtf8(i, "select"), FunctionTemplate::New(i, Select));


    Local<Object> obj = ds_template->NewInstance();
    obj->SetInternalField(0, External::New(i, datastream));
    // Create and Return this newly created object
    info.GetReturnValue().Set(obj);
  } else {
    fprintf(stderr, "datastream not found with id %d \n", datastream_id);
  }
}

void Datastream::JS::GetX(Local<String> property,                                               
		 const PropertyCallbackInfo<Value> &info) {

  Local<Object> self = info.Holder();

  Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));

  void* ptr = wrap->Value();

  int value = 5;

  info.GetReturnValue().Set(Integer::New(info.GetIsolate(), value));
}    

void Datastream::JS::GetDatapoints(Local<String> property,
				   const PropertyCallbackInfo<Value> &info) {

  std::cout << "GetDatapoints" << std::endl;
  Isolate * isolate = info.GetIsolate();
 
  Local<Object> self = info.Holder();
  Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
  void* ptr = wrap->Value();

  Datastream * datastream = static_cast<Datastream*>(ptr);
  Handle<Array> array = datastream->toV8Array(isolate);
  info.GetReturnValue().Set(array);                                                                                                                    

}   

void Datastream::JS::Select(const FunctionCallbackInfo<Value>& info ) {
  Isolate * isolate = info.GetIsolate();
  
  
  //v8::Isolate* i = v8::Isolate::GetCurrent();
  
  Handle<Value> arg1 = info[0];
  Handle<Value> arg2 = info[1];

  //if (!arg1.IsEmpty() && !arg2.IsEmpty() && arg1->IsDate() && arg2->IsDate()) {
    std::cout << " IS DATE !!!" << std::endl;
  
    //Local<Object> self = info.Holder();
    //info.This()->Get(String::NewFromUtf8(isolate, "datapoints"))->Int32Value();
    //Handle<Array> array = ->AsArray();
    Handle<Value> array_Value = info.This()->Get(String::NewFromUtf8(isolate, "datapoints"));
    

    //Handle<v8::Array> arr = Handle<v8::Array>::Cast(val); 
    //v8::String::Utf8Value key(val->Get(0));
    //v8::String::Utf8Value value(val->Get(1));
    
    //std::cout << "key: "<< *key << " value: "<< *value << std::endl;
    //std::cout << "IsEmpty: "<< val.IsEmpty() << std::endl;
    //std::cout << "IsTypedArray: "<< val->IsTypedArray() << std::endl;
    //std::cout << "IsArray: "<< val->IsArray() << std::endl;
    //Handle<Array> array = self.Get(v8::String::NewFromUtf8("datapoints"))->ToArray();
    
    //}
  
}

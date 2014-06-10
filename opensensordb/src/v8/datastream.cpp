#include "datastream.h"

#include "db.h"


bool Datastream::load(int id) {
        return DB::GetInstance()->loadDatastream(this, id);  
}


bool Datastream::loadContiguous(int id) {
        return DB::GetInstance()->loadDatastreamContiguous(this, id);  
}


Handle<Array> Datastream::toV8Array(Isolate * i) {
        std::cout << "toV8Array"  << std::endl;
  const std::vector<Datastream::Datarow*>& datarows = this->datarows;
  
  Handle<Array> datapoints_array = Array::New(i, datapoints.size());
  
  
  int datapoints_count = 0;
  for (std::vector<Pair>::const_iterator it = datapoints.begin(); it != datapoints.end(); ++it, datapoints_count++) {

          const Pair& p = (*it);    

          std::cout << "p time " << p.time << " date " << (double)p.time << std::endl;

          //"date":"1970-01-01T00:00:00.000Z"
          Handle<Object> obj = Object::New(i);

          obj->Set(v8::String::NewFromUtf8(i, "date"), Date::New(i, (double)p.time * 1000));
        
          obj->Set(v8::String::NewFromUtf8(i, "value"),
                   Number::New(i, p.value));

      //Handle<Object> obj = Object::New(i);
      //pair_array->Set(0, String::New(i, p.strtime));
      //pair_array->Set(1, Number::New(i, p.value));
      
      datapoints_array->Set(datapoints_count, obj);

  }
  std::cout << "toV8Array datapoints count: " << datapoints_count << " " << datapoints.size() << std::endl;
  
  return datapoints_array;
    
}



Handle<Array> Datastream::toV8ArrayContiguous(Isolate * i) {
        std::cout << "toV8ArrayContiguous"  << std::endl;
        
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
                        pair_array->Set(0, String::NewFromUtf8(i, p.strtime));
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

  Datastream * datastream = new Datastream();

    
  if (datastream) {
    
    datastream->print();
    
    // First Way : create the datapoints one time and store it into javascript Datastream object.
    //Handle<Array> array = datastream->toV8Array(i);
    Handle<Array> array = Array::New(i, 3);
    ds_template->Set(String::NewFromUtf8(i, "datapoints"), array);
    ds_template->Set(String::NewFromUtf8(i, "datapoints_contiguous"), array);

    // Second way : call the c++ function "GetDatapoints" (and create a javascript array) each time "datapoints" is called in javascript
    ds_template->SetAccessor(String::NewFromUtf8(i, "datapointsB"), GetDatapoints);
    ds_template->SetAccessor(String::NewFromUtf8(i, "x"), GetX);


    // set a javascript function WIP
    ds_template->Set(String::NewFromUtf8(i, "load"), FunctionTemplate::New(i, Load));
    ds_template->Set(String::NewFromUtf8(i, "select"), FunctionTemplate::New(i, Select));


    Local<Object> obj = ds_template->NewInstance();
    obj->SetInternalField(0, External::New(i, datastream));
    // Create and Return this newly created object
    info.GetReturnValue().Set(obj);
  } else {
    fprintf(stderr, "datastream not found with id %d \n", datastream_id);
  }
}

void Datastream::JS::Load(const FunctionCallbackInfo<Value>& info ) {

        std::cout << "Datastream load..."  << std::endl;

        Isolate * isolate = info.GetIsolate();
        
        Local<Object> self = info.Holder();
        Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
        void* ptr = wrap->Value();
        
        Datastream * datastream = static_cast<Datastream*>(ptr);
        

        const Handle<Value>& arg1 = info[0]; 
        
        const int datastream_id = !arg1.IsEmpty() && arg1->IsNumber() ? arg1->Int32Value() : 0;
        std::cout << "Datastream Load: datastream_id: " << datastream_id << std::endl;
        std::cout << "Datastream Load: datastream: " << datastream << std::endl;
        std::cout << "Datastream->load:" << datastream->load(datastream_id) << std::endl;
        std::cout << "Datastream->loadContiguous:" << datastream->loadContiguous(datastream_id) << std::endl;

        Handle<Array> array = datastream->toV8Array(isolate);
        info.This()->Set(String::NewFromUtf8(isolate, "datapoints"), array);
        
        Handle<Array> array_c = datastream->toV8ArrayContiguous(isolate);
        info.This()->Set(String::NewFromUtf8(isolate, "datapoints_contiguous"), array_c);

}

void Datastream::JS::Select(const FunctionCallbackInfo<Value>& info ) {

        Isolate * isolate = info.GetIsolate();
        
        Local<Object> self = info.Holder();
        Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
        void* ptr = wrap->Value();
        
        Datastream * datastream = static_cast<Datastream*>(ptr);
        
  
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


void Datastream::JS::GetX(Local<String> property,                                               
		 const PropertyCallbackInfo<Value> &info) {

        //Local<Object> self = info.Holder();

        //Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
        
        //void* ptr = wrap->Value();
        
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


#include "datastream.h"

#include "db.h"

// Datapoint

void Datapoint::print() const {
        std::cout << "strtime: " << strtime
                  << " time_t: " << time
                  << " value: " << value
                  << std::endl;
}

// Datastream

Datastream::Datastream() :
        id(0),
        ownerId(0),
        current(0x0)
{
        memset(name, 0, sizeof(name));
        memset(description, 0, sizeof(description));
}
	
Datastream::~Datastream() {
        clear();
        clearContiguous();
}


bool Datastream::load(int id) {
        clear();
        return DB::GetInstance()->loadDatastream(this, id);  
}


bool Datastream::loadContiguous(int id) {
        clearContiguous();
        return DB::GetInstance()->loadDatastreamContiguous(this, id);  
}

void Datastream::print() const {
        std::cout << "datapoints size: " << datapoints.size()
                  << " datarows size " << datarows.size()
                  << std::endl;
}

void Datastream::clear() {

        id = 0;
        ownerId = 0;
        current = 0x0;
 
        memset(name, 0, sizeof(name));
        memset(description, 0, sizeof(description));

        for (std::vector<Datarow*>::iterator it = datarows.begin(); it != datarows.end(); ++it) {
                delete (*it);
        };

        datarows.clear();

        //TODO free all strtime
}


void Datastream::clearContiguous() {

        for (std::vector<Datarow*>::iterator it = datarows.begin(); it != datarows.end(); ++it) {
                delete (*it);
        };

        datarows.clear();

        //TODO free all strtime
}

Handle<Array> Datastream::toV8Array(Isolate * i) const {
        
        Handle<Array> datapoints_array = Array::New(i, datapoints.size());
        
        int datapoints_count = 0;
        for (std::vector<Datapoint>::const_iterator it = datapoints.begin(); it != datapoints.end(); ++it, datapoints_count++) {                
                const Datapoint& d = (*it);    
                
                Handle<Object> obj = Object::New(i);
                
                obj->Set(v8::String::NewFromUtf8(i, "date"), Date::New(i, (double)d.time * 1000));
                obj->Set(v8::String::NewFromUtf8(i, "value"), Number::New(i, d.value));
                
                datapoints_array->Set(datapoints_count, obj);
        }
        
        //std::cout << "toV8Array datapoints count: " << datapoints_count << " " << datapoints.size() << std::endl;
        
        return datapoints_array;
}

Handle<Array> Datastream::toV8ArrayContiguous(Isolate * i) {
        //std::cout << "toV8ArrayContiguous"  << std::endl;
        
        const std::vector<Datastream::Datarow*>& datarows = this->datarows;
        
        Handle<Array> parent_array = Array::New(i, datarows.size());
        
        int datarows_count = 0;
        for (std::vector<Datastream::Datarow*>::const_iterator it = datarows.begin(); it != datarows.end(); ++it, datarows_count++) {
                const Datastream::Datarow * dr = (*it);
    
                Handle<Array> rows_array = Array::New(i, dr->size());
                parent_array->Set(datarows_count, rows_array);
                
                int datapoints_count = 0;
                for (std::vector<Datapoint>::const_iterator itb = dr->begin(); itb != dr->end(); ++itb, datapoints_count++) {
                        const Datapoint& d = (*itb);
                        
                        Handle<Array> pair_array = Array::New(i, 2);
                        pair_array->Set(0, String::NewFromUtf8(i, d.strtime));
                        pair_array->Set(1, Number::New(i, d.value));
                        
                        rows_array->Set(datapoints_count, pair_array);
                }                             
        }
        
        return parent_array;
}

// Datastream::JS

Local<Object> Datastream::JS::GetNewInstance(Isolate * i) {

        Handle<ObjectTemplate> ds_template = ObjectTemplate::New();
        ds_template->SetInternalFieldCount(1);
        
        // First Way : create the datapoints one time and store it into javascript Datastream object.
        Handle<Array> array = Array::New(i, 3);
        //ds_template->Set(String::NewFromUtf8(i, "datapoints"), array);
        //ds_template->Set(String::NewFromUtf8(i, "datapoints_contiguous"), array);
        
        // Second way : call the c++ function "GetDatapoints" (and create a javascript array) each time "datapoints" is called in javascript
        //ds_template->SetAccessor(String::NewFromUtf8(i, "datapointsFromAccessor"), GetDatapoints);
        
        // set a javascript function WIP
        ds_template->Set(String::NewFromUtf8(i, "load"), FunctionTemplate::New(i, Load));
        ds_template->Set(String::NewFromUtf8(i, "select"), FunctionTemplate::New(i, Select));
        
        Local<Object> obj = ds_template->NewInstance();
        return obj;
}

void Datastream::JS::SetupObject(Local<Object> obj, const Datastream * d, Isolate* i) {

        if (!d) {
                fprintf(stderr, "Datastream::JS::SetupObject error, Datastream is null");
                return;
        }

        obj->Set(String::NewFromUtf8(i, "id"), Number::New(i, d->id));
        obj->Set(String::NewFromUtf8(i, "ownerId"), Number::New(i, d->ownerId));
        obj->Set(String::NewFromUtf8(i, "name"), String::NewFromUtf8(i, d->name));

        Handle<Array> array = d->toV8Array(i);
        obj->Set(String::NewFromUtf8(i, "datapoints"), array);

        //obj->Set(String::NewFromUtf8(i, "select"), FunctionTemplate::New(i, Select));
}

void Datastream::JS::Register(Handle<ObjectTemplate> global, Isolate* i) {
        global->Set(String::NewFromUtf8(i, "Datastream"), FunctionTemplate::New(i, Constructor));
}


void Datastream::JS::Constructor(const FunctionCallbackInfo<Value>& info) {
        v8::Isolate* i = v8::Isolate::GetCurrent();


        const Handle<Value>& arg1 = info[0]; 
        
        const int datastream_id = !arg1.IsEmpty() && arg1->IsNumber() ? arg1->Int32Value() : 0;
  
        Datastream * datastream = new Datastream();
        
        if (datastream) {
                /*
                Handle<ObjectTemplate> ds_template = ObjectTemplate::New();
                ds_template->SetInternalFieldCount(1);
    
                // First Way : create the datapoints one time and store it into javascript Datastream object.
                //Handle<Array> array = datastream->toV8Array(i);
                Handle<Array> array = Array::New(i, 3);
                ds_template->Set(String::NewFromUtf8(i, "datapoints"), array);
                ds_template->Set(String::NewFromUtf8(i, "datapoints_contiguous"), array);
                
                // Second way : call the c++ function "GetDatapoints" (and create a javascript array) each time "datapoints" is called in javascript
                ds_template->SetAccessor(String::NewFromUtf8(i, "datapointsFromAccessor"), GetDatapoints);
                //ds_template->SetAccessor(String::NewFromUtf8(i, "x"), GetX);
                

                // set a javascript function WIP
                ds_template->Set(String::NewFromUtf8(i, "load"), FunctionTemplate::New(i, Load));
                ds_template->Set(String::NewFromUtf8(i, "select"), FunctionTemplate::New(i, Select));
                

                Local<Object> obj = ds_template->NewInstance();
                */

                Local<Object> obj = GetNewInstance(i);

                obj->SetInternalField(0, External::New(i, datastream));
                // Create and Return this newly created object
                info.GetReturnValue().Set(obj);
                
                //datastream->print();

        } else {
                fprintf(stderr, "datastream not found with id %d \n", datastream_id);
        }
}

void Datastream::JS::Load(const FunctionCallbackInfo<Value>& info ) {

        Isolate * isolate = info.GetIsolate();
        
        Local<Object> self = info.Holder();
        Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
        void* ptr = wrap->Value();
        
        Datastream * datastream = static_cast<Datastream*>(ptr);
        
        const Handle<Value>& arg1 = info[0]; 
        
        //const int datastream_id = !arg1.IsEmpty() && arg1->IsNumber() ? arg1->Int32Value() : 0;
        const int datastream_id = !arg1.IsEmpty() ? arg1->Int32Value() : 0;

        //std::cout << "Datastream Load: !arg1.IsEmpty() " << !arg1.IsEmpty() << std::endl;
        //std::cout << "Datastream Load: arg1->IsNumber() " << arg1->IsNumber() << std::endl;
        //std::cout << "Datastream Load: datastream_id: " << datastream_id << std::endl;
        //std::cout << "Datastream Load: datastream: " << datastream << std::endl;
        //std::cout << "Datastream->load(datastream_id): " << datastream->load(datastream_id) << std::endl;
        //std::cout << "Datastream->loadContiguous(datastream_id): " << datastream->loadContiguous(datastream_id) << std::endl;

        datastream->load(datastream_id);
        datastream->loadContiguous(datastream_id);

        //Fill this "Datastream" javascript object With properties
        SetupObject(info.This(), datastream, isolate);
        
        /*        
          Handle<Array> array = datastream->toV8Array(isolate);
          info.This()->Set(String::NewFromUtf8(isolate, "datapoints"), array);
          
          Handle<Array> array_c = datastream->toV8ArrayContiguous(isolate);
          info.This()->Set(String::NewFromUtf8(isolate, "datapoints_contiguous"), array_c);
        
        */
}

void Datastream::JS::Select(const FunctionCallbackInfo<Value>& info ) {

        Isolate * isolate = info.GetIsolate();
        
        Local<Object> self = info.Holder();
        Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
        void* ptr = wrap->Value();
        
        Datastream * datastream = static_cast<Datastream*>(ptr);
        
        Handle<Value> arg1 = info[0];
        Handle<Value> arg2 = info[1];
        
        Handle<String> inv = String::NewFromUtf8(isolate, "Invalid Date");
               
        Handle<Array> datapoints_array = Array::New(isolate, 0);

        bool ok = !arg1.IsEmpty() && !arg2.IsEmpty();
        ok = ok && !arg1->IsUndefined() && !arg2->IsUndefined();
        ok = ok && arg1->IsDate() && arg2->IsDate();

        // Convert args to string value to check if date is valid        
        {
                const v8::String::Utf8Value invalid_date(inv);

                v8::String::Utf8Value str_arg1(info[0]);
                v8::String::Utf8Value str_arg2(info[1]);
                ok = ok && strcmp(*str_arg1, *invalid_date) != 0 && strcmp(*str_arg2, *invalid_date) != 0;
        }

        if (ok) {
                const double start_t = arg1->NumberValue();
                const long start_ut = (long)start_t / 1000;
                const double end_t = arg2->NumberValue();
                const long end_ut = (long)end_t / 1000;

                const std::vector<Datapoint>& datapoints =  datastream->datapoints;
                
                size_t datapoints_count = 0;
                for (std::vector<Datapoint>::const_iterator i = datapoints.begin(); i != datapoints.end(); ++i) {
                        const Datapoint& d = (*i);

                        if (d.time >= start_ut && d.time <= end_ut) {
                                
                                Handle<Object> obj = Object::New(isolate);
                                
                                obj->Set(String::NewFromUtf8(isolate, "date"), Date::New(isolate, (double)d.time * 1000));
                                obj->Set(String::NewFromUtf8(isolate, "value"), Number::New(isolate, d.value));
                                
                                datapoints_array->Set(datapoints_count, obj);
                                ++datapoints_count;
                        }
                        
                }
        }

        info.GetReturnValue().Set(datapoints_array);        
}

void Datastream::JS::ComputationTime(Local<String> property,
				   const PropertyCallbackInfo<Value> &info) {

        //std::cout << "GetDatapointsAccessor" << std::endl;
        Isolate * isolate = info.GetIsolate();
        
        Local<Object> self = info.Holder();
        Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
        void* ptr = wrap->Value();
        
        Datastream * datastream = static_cast<Datastream*>(ptr);
        Handle<Array> array = datastream->toV8Array(isolate);

        time_t total_time = 0;

	//const std::vector<Datarow*>& datarows = datastream->datarows;
	std::vector<Datarow*> datarows = std::vector<Datarow*>();
        
        Datarow * a = new Datarow;
        Datarow * b = new Datarow;
        Datapoint da,db, dc,dd,de,df;

        da.time = 0;
        db.time = 10;

        dc.time = 30;
        dd.time = 40;
        de.time = 50;
        df.time = 60;

        a->push_back(da);
        a->push_back(db);

        b->push_back(dc);
        b->push_back(dd);
        b->push_back(de);
        b->push_back(df);
        
        /*
        for (std::vector<Datarow*>::iterator i = datarows.begin(); i != datarows.end(); ++i) {
                Datarow* row = (*i);
                
                Datarow& drow = *row;
                if (row->empty() || row->size() < 2) {
                        continue;
                }
                
                size_t counter = 0 ;
                Datapoint& last = drow[counter];
                counter++;
                Datapoint& current = drow[counter];
                do {
                        time_t delta = current.time - last.time;
                        
                        std::cout << "delta: " << delta << std::endl;
                        total_time += delta;

                        last = current;
                        counter++;
                        current = drow[counter];
                } while ();
                std::cout << "total_time: " << total_time << std::endl;

        }

        */

        info.GetReturnValue().Set(Number::New(isolate, total_time));
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

        //std::cout << "GetDatapointsAccessor" << std::endl;
        Isolate * isolate = info.GetIsolate();
        
        Local<Object> self = info.Holder();
        Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
        void* ptr = wrap->Value();
        
        Datastream * datastream = static_cast<Datastream*>(ptr);
        Handle<Array> array = datastream->toV8Array(isolate);
        info.GetReturnValue().Set(array);
}   


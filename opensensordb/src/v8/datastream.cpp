#include "datastream.h"

#include "db.h"

#include <cmath>

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
        std::cout << "Datastream::Datastream()" << std::endl;
        memset(name, 0, sizeof(name));
        memset(description, 0, sizeof(description));
}
	
Datastream::~Datastream() {
        std::cout << "Datastream::~Datastream()" << std::endl;        
        clear();
}

bool Datastream::load(int id) {
        clear();
        return DB::GetInstance()->loadDatastream(this, id);  
}

bool Datastream::loadFromRange(int id, time_t start, time_t end) {
        clear();
        return DB::GetInstance()->loadDatastreamFromRange(this, id, start, end);  
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
        }

        datarows.clear();

        //TODO free all strtime
}

Handle<Array> Datastream::toV8Array(Isolate * i) const {
        
        Handle<Array> datapoints_array = Array::New(i, datapoints.size());
        
        int datapoints_count = 0;
        for (std::vector<Datapoint>::const_iterator it = datapoints.begin(); it != datapoints.end(); ++it, datapoints_count++) {                
                const Datapoint& d = (*it);    
                
                Handle<Object> obj = Object::New(i);
                
                obj->Set(String::NewFromUtf8(i, "date"), Date::New(i, (double)d.time * 1000));
                obj->Set(String::NewFromUtf8(i, "value"), Number::New(i, d.value));
                
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

// Simple way

void Datastream::appendDatapoints(char * strtime, time_t time, double value) {
        Datapoint d;
        strcpy(d.strtime, strtime);
        d.time = time;
        d.value = value;
        
        datapoints.push_back(d);
}

// Contiguous way

void Datastream::beginDatarow() {
        current = new Datarow;
}

void Datastream::append(char * strtime, time_t time, double value)
{
        if (!current) {
                beginDatarow();
        }
        Datapoint d;
        strcpy(d.strtime, strtime);
        d.time = time;
        d.value = value;
        current->push_back(d);
}

void Datastream::endDatarow()
{
        if (current) {
                datarows.push_back(current);
                current = 0;
        }
}


Handle<Array> Datastream::select(Isolate * iso, time_t start, time_t end) const
{

        Handle<Array> datapoints_array = Array::New(iso, 0);

        size_t datapoints_count = 0;
        for (std::vector<Datapoint>::const_iterator i = datapoints.begin(); i != datapoints.end(); ++i) {
                const Datapoint& d = (*i);
                
                if (d.time >= start && d.time <= end) {
                        
                        Handle<Object> obj = Object::New(iso);
                        
                        obj->Set(String::NewFromUtf8(iso, "date"), Date::New(iso, (double)d.time * 1000));
                        obj->Set(String::NewFromUtf8(iso, "value"), Number::New(iso, d.value));
                        
                        datapoints_array->Set(datapoints_count, obj);
                        ++datapoints_count;
                }                
        }

        return datapoints_array;
}

Handle<Array> Datastream::select2(Isolate * iso, time_t start, time_t end) const
{

        //std::cout << "Datastream::select2: " << std::endl;
        Handle<Array> datarows_array = Array::New(iso, 0);

        //std::cout << "data row size: " << datarows.size() << std::endl;                
        size_t datarows_count = 0;
        for (std::vector<Datarow*>::const_iterator it = datarows.begin(); it != datarows.end(); ++it) {
                const Datarow * dr = (*it);
                size_t datapoints_count = 0;
                Handle<Array> datapoints_array = Array::New(iso, 0);
                bool has_value = false;

                //std::cout << "current row size: " << dr->size() << std::endl;                
                for (std::vector<Datapoint>::const_iterator it2 = dr->begin(); it2 != dr->end(); ++it2) {

                        const Datapoint& dp = (*it2);
                        
                        if (dp.time >= start && dp.time <= end) {
                                
                                Handle<Object> obj = Object::New(iso);
                                
                                obj->Set(String::NewFromUtf8(iso, "date"), Date::New(iso, (double)dp.time * 1000));
                                obj->Set(String::NewFromUtf8(iso, "value"), Number::New(iso, dp.value));
                                
                                //std::cout << "datapoint : time & value: " << dp.time << " - " << dp.value << std::endl;
                                datapoints_array->Set(datapoints_count, obj);
                                ++datapoints_count;
                                has_value = true;
                        }
                }
                


                if (has_value) {
                        datarows_array->Set(datarows_count, datapoints_array);
                        datarows_count++;
                }
        }

        //std::cout << "Datastream::select2 datarow A result: " << datarows_count << std::endl;

        return datarows_array;
}

// Datastream::JS

Handle<ObjectTemplate> Datastream::JS::GetNewTemplate(Isolate * i)
{

        Handle<ObjectTemplate> ds_template = ObjectTemplate::New();
        ds_template->SetInternalFieldCount(1);
        
        // First Way : create the datapoints one time and store it into javascript Datastream object.
        //ds_template->Set(String::NewFromUtf8(i, "datapoints"), array);
        //ds_template->Set(String::NewFromUtf8(i, "datapoints_contiguous"), array);
        
        // Second way : call the c++ function "GetDatapoints" (and create a javascript array) each time "datapoints" is called in javascript
        //ds_template->SetAccessor(String::NewFromUtf8(i, "datapointsFromAccessor"), GetDatapoints);
        
        // set a javascript function WIP
        ds_template->Set(String::NewFromUtf8(i, "load"), FunctionTemplate::New(i, Load));
        ds_template->Set(String::NewFromUtf8(i, "select"), FunctionTemplate::New(i, SelectZ));
        ds_template->Set(String::NewFromUtf8(i, "select2"), FunctionTemplate::New(i, Select2));
        ds_template->Set(String::NewFromUtf8(i, "computationTime"), FunctionTemplate::New(i, ComputationTime));
        ds_template->Set(String::NewFromUtf8(i, "totalTime"), FunctionTemplate::New(i, TotalTime));

        return ds_template;
}

void Datastream::JS::SetupObject(Local<Object> obj, const Datastream * d, Isolate* i)
{

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

void Datastream::JS::Register(Handle<ObjectTemplate> global, Isolate* i)
{
        global->Set(String::NewFromUtf8(i, "Datastream"), FunctionTemplate::New(i, Constructor));
}


void Datastream::JS::Constructor(const FunctionCallbackInfo<Value>& info)
{
        Isolate* i = Isolate::GetCurrent();

        const Handle<Value>& arg1 = info[0]; 
        
        const int datastream_id = !arg1.IsEmpty() && arg1->IsNumber() ? arg1->Int32Value() : 0;
  
        Datastream * datastream = new Datastream();
        
        if (datastream) {

                Handle<ObjectTemplate> ds_template = GetNewTemplate(i);
                Local<Object> obj = ds_template->NewInstance();

                obj->SetInternalField(0, External::New(i, datastream));
                AddToRef(datastream);
                // Create and Return this newly created object
                info.GetReturnValue().Set(obj);
                
                //datastream->print();

        } else {
                fprintf(stderr, "datastream not found with id %d \n", datastream_id);
        }
}

void Datastream::JS::Load(const FunctionCallbackInfo<Value>& info)
{
        Isolate * isolate = info.GetIsolate();
        
        Local<Object> self = info.Holder();
        Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
        void* ptr = wrap->Value();
        
        Datastream * datastream = static_cast<Datastream*>(ptr);
        
        const Handle<Value>& arg1 = info[0];
        const Handle<Value>& arg2 = info[1];
        const Handle<Value>& arg3 = info[2];

        bool has_id = !arg1.IsEmpty();
        bool has_date = !arg2.IsEmpty() && !arg3.IsEmpty();
                
        if (has_id && has_date) {
                const int datastream_id = arg1->Int32Value();
                if (has_date) {
                        // Get only data between the interval
                        const double start_t = arg2->NumberValue();
                        const long start_ut = (long)start_t / 1000;
                        const double end_t = arg3->NumberValue();
                        const long end_ut = (long)end_t / 1000;

                        datastream->loadFromRange(datastream_id, start_ut, end_ut);
                } else {
                        // Get all data
                        datastream->load(datastream_id);
                }
                // Fill this "Datastream" javascript object With properties
                SetupObject(info.This(), datastream, isolate);
        }        
}

/*
void Datastream::JS::Load(const FunctionCallbackInfo<Value>& info)
{

        Isolate * isolate = info.GetIsolate();
        
        Local<Object> self = info.Holder();
        Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
        void* ptr = wrap->Value();
        
        Datastream * datastream = static_cast<Datastream*>(ptr);
        
        const Handle<Value>& arg1 = info[0]; 
        
        //const int datastream_id = !arg1.IsEmpty() && arg1->IsNumber() ? arg1->Int32Value() : 0;
        const int datastream_id = !arg1.IsEmpty() ? arg1->Int32Value() : 0;

        bool b = datastream->load(datastream_id);

        
        //Fill this "Datastream" javascript object With properties
        SetupObject(info.This(), datastream, isolate);
}
*/

void Datastream::JS::Select(const FunctionCallbackInfo<Value>& info)  {

        Isolate * isolate = info.GetIsolate();
        
        Local<Object> self = info.Holder();
        Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
        void* ptr = wrap->Value();
        
        Datastream * datastream = static_cast<Datastream*>(ptr);
        
        Handle<Value> arg1 = info[0];
        Handle<Value> arg2 = info[1];
        
        Handle<String> inv = String::NewFromUtf8(isolate, "Invalid Date");

        bool ok = !arg1.IsEmpty() && !arg2.IsEmpty();
        ok = ok && !arg1->IsUndefined() && !arg2->IsUndefined();
        ok = ok && arg1->IsDate() && arg2->IsDate();

        // Convert args to string value to check if date is valid        
        {
                const String::Utf8Value invalid_date(inv);

                String::Utf8Value str_arg1(info[0]);
                String::Utf8Value str_arg2(info[1]);
                ok = ok && strcmp(*str_arg1, *invalid_date) != 0 && strcmp(*str_arg2, *invalid_date) != 0;
        }

        if (ok) {

                const double start_t = arg1->NumberValue();
                const long start_ut = (long)start_t / 1000;
                const double end_t = arg2->NumberValue();
                const long end_ut = (long)end_t / 1000;

                Handle<Array> datapoints_array = Array::New(isolate, 0);

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
                info.GetReturnValue().Set(datapoints_array);
        }
}


void Datastream::JS::SelectZ(const FunctionCallbackInfo<Value>& info)  {

        Isolate * isolate = info.GetIsolate();
        
        Local<Object> self = info.Holder();
        Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
        void* ptr = wrap->Value();
        
        Datastream * datastream = static_cast<Datastream*>(ptr);
        
        Handle<Value> arg1 = info[0];
        Handle<Value> arg2 = info[1];
        
        Handle<String> inv = String::NewFromUtf8(isolate, "Invalid Date");
               


        bool ok = !arg1.IsEmpty() && !arg2.IsEmpty();
        ok = ok && !arg1->IsUndefined() && !arg2->IsUndefined();
        ok = ok && arg1->IsDate() && arg2->IsDate();

        // Convert args to string value to check if date is valid        
        {
                const String::Utf8Value invalid_date(inv);

                String::Utf8Value str_arg1(info[0]);
                String::Utf8Value str_arg2(info[1]);
                ok = ok && strcmp(*str_arg1, *invalid_date) != 0 && strcmp(*str_arg2, *invalid_date) != 0;
        }

        if (ok) {
                const double start_t = arg1->NumberValue();
                const time_t start_ut = (time_t)start_t / 1000;
                const double end_t = arg2->NumberValue();
                const time_t end_ut = (time_t)end_t / 1000;

                Handle<Array> datapoints_array = datastream->select(isolate, start_ut, end_ut);
                info.GetReturnValue().Set(datapoints_array);
        }


}

void Datastream::JS::Select2(const FunctionCallbackInfo<Value>& info)  {

        Isolate * isolate = info.GetIsolate();
        
        Local<Object> self = info.Holder();
        Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
        void* ptr = wrap->Value();
        
        Datastream * datastream = static_cast<Datastream*>(ptr);
        
        Handle<Value> arg1 = info[0];
        Handle<Value> arg2 = info[1];
        
        Handle<String> inv = String::NewFromUtf8(isolate, "Invalid Date");

        bool ok = !arg1.IsEmpty() && !arg2.IsEmpty();
        ok = ok && !arg1->IsUndefined() && !arg2->IsUndefined();
        ok = ok && arg1->IsDate() && arg2->IsDate();

        // Convert args to string value to check if date is valid        
        {
                const String::Utf8Value invalid_date(inv);

                String::Utf8Value str_arg1(info[0]);
                String::Utf8Value str_arg2(info[1]);
                ok = ok && strcmp(*str_arg1, *invalid_date) != 0 && strcmp(*str_arg2, *invalid_date) != 0;
        }

        if (ok) {
                const double start_t = arg1->NumberValue();
                const time_t start_ut = (time_t)start_t / 1000;
                const double end_t = arg2->NumberValue();
                const time_t end_ut = (time_t)end_t / 1000;

                Handle<Array> datapoints_array = datastream->select2(isolate, start_ut, end_ut);
                info.GetReturnValue().Set(datapoints_array);
        }


}




struct DHMS {
        time_t days;
        time_t hours;
        time_t minutes;
        time_t seconds;
};

// Time in second (t) to Day Hour Minute Second (DHMS)

void secondToDHMS(time_t t, DHMS& dhms) {

        dhms.days = dhms.hours = dhms.minutes = dhms.seconds = 0;

        time_t temp = t;
        dhms.days = temp / (24 * 3600);
        temp = temp - (dhms.days * 24 * 3600);
        dhms.hours = temp / 3600;
        temp = temp - (dhms.hours * 3600);
        dhms.minutes = temp / 60;
        dhms.seconds = temp - (dhms.minutes * 60);

        //std::cout << "days: " << days << " hours: " << hours << " minutes: " << minutes << " seconds: " << seconds << std::endl;
}

void Datastream::JS::ComputationTime(const FunctionCallbackInfo<Value>& info) {

        //std::cout << "Datastream::JS::ComputationTime" << std::endl;

        Isolate * isolate = info.GetIsolate();
        
        Local<Object> self = info.Holder();
        Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
        void* ptr = wrap->Value();
        
        Datastream * datastream = static_cast<Datastream*>(ptr);

        time_t total_time = 0;

	std::vector<Datarow*>& datarows = datastream->datarows;

        for (std::vector<Datarow*>::iterator i = datarows.begin(); i != datarows.end(); ++i) {
                Datarow* row = (*i);
                
                Datarow& drow = *row;
                if (row->empty() || row->size() < 2) {
                        continue;
                }
                
                size_t counter = 0 ;
                Datapoint& last = drow[counter];

                while (counter < drow.size() ) {

                        Datapoint& current = drow[counter];

                        time_t delta = current.time - last.time;

                        total_time += delta;

                        last = current;
                        ++counter;
                }
        }

        //std::cout << "cpp total_time: " << total_time << std::endl;

        DHMS t; // elapsed_time with "days", "hours", "minutes", "seconds"
        secondToDHMS(total_time, t);

        Handle<Object> time_obj = Object::New(isolate);
        time_obj->Set(String::NewFromUtf8(isolate, "days"), Number::New(isolate, t.days));
        time_obj->Set(String::NewFromUtf8(isolate, "hours"), Number::New(isolate, t.hours));
        time_obj->Set(String::NewFromUtf8(isolate, "minutes"), Number::New(isolate, t.minutes));
        time_obj->Set(String::NewFromUtf8(isolate, "seconds"), Number::New(isolate, t.seconds));
        
        //std::cout << "days: " << t.days << " hours: " << t.hours << " minutes: " << t.minutes << " seconds: " << t.seconds << std::endl;

        info.GetReturnValue().Set(time_obj);
}   

void Datastream::JS::TotalTime(const FunctionCallbackInfo<Value>& info) {

        Isolate * isolate = info.GetIsolate();
        
        Local<Object> self = info.Holder();
        Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
        void* ptr = wrap->Value();
        
        Datastream * datastream = static_cast<Datastream*>(ptr);

        //Handle<Array> array = datastream->toV8Array(isolate);

        //time_t total_time = 0;

        time_t begin_time = 0;
        time_t last_time = 0;
        bool first = true;


	std::vector<Datarow*>& datarows = datastream->datarows;
        for (std::vector<Datarow*>::iterator i = datarows.begin(); i != datarows.end(); ++i) {
                Datarow* row = (*i);
                
                Datarow& drow = *row;
                if (row->empty() || row->size() < 2) {
                        continue;
                }
                
                size_t counter = 0 ;
                Datapoint& last = drow[counter];
                //counter++;

                while (counter < drow.size() ) {

                        Datapoint& current = drow[counter];
                        
                        if (first) {
                                begin_time = current.time; 
                                first = false;
                        }

                        last = current;

                        ++counter;
                        last_time = last.time;
                }
        }

        //std::cout << "cpp total_time: " << total_time << std::endl;

        DHMS t; // elapsed_time with "days", "hours", "minutes", "seconds"
        secondToDHMS(last_time - begin_time, t);

        Handle<Object> time_obj = Object::New(isolate);
        time_obj->Set(String::NewFromUtf8(isolate, "days"), Number::New(isolate, t.days));
        time_obj->Set(String::NewFromUtf8(isolate, "hours"), Number::New(isolate, t.hours));
        time_obj->Set(String::NewFromUtf8(isolate, "minutes"), Number::New(isolate, t.minutes));
        time_obj->Set(String::NewFromUtf8(isolate, "seconds"), Number::New(isolate, t.seconds));
        
        info.GetReturnValue().Set(time_obj);
}

void Datastream::JS::GetX(Local<String> property,                                               
                          const PropertyCallbackInfo<Value> &info) {
        
        int fake_value_x = 5;
        
        info.GetReturnValue().Set(Integer::New(info.GetIsolate(), fake_value_x));
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

std::vector<Datastream*> Datastream::JS::references;

void Datastream::JS::AddToRef(Datastream * datastream) {
        references.push_back(datastream);
}

void Datastream::JS::DeleteAllRef() {
        
        for (std::vector<Datastream*>::iterator it = references.begin(); it != references.end(); ++it) {
                delete (*it);
        }
        
        references.clear();        
}

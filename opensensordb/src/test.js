

var start_date = new Date(args.start);
var end_date = new Date(args.end);

var d = new Datastream();
d.load(args.id);

var points = d.select(start_date, end_date);

var avg = 0;

for (var i = 0; i < points.length; i++)
    avg += points[i].value;

if (points.length > 0)
   avg /= points.length;

JSON.stringify(avg);


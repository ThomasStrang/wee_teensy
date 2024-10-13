import csv
import json

with open("thing2.csv", "r") as f:
	astr = f.read()


values = astr.split(",")

num_values = len(values)>>1
print(num_values)

print(values[0])
print(values[1])

times_and_readings=[]

for i in range(num_values):
	time_with_random_thing=values[i*2]
	time_to_parse_reading=time_with_random_thing[:2]

	time_of_reading=time_with_random_thing[-5:]
	reading=values[i*2 + 1]
	times_and_readings.append([time_of_reading, reading])

# with open('out.csv', 'w', newline="") as f:
#     writer = csv.writer(f)
#     writer.writerows([["time_of_reading","reading"]])
#     writer.writerows(times_and_readings)

HIGH_TRIGGER_FOR_CENTRE=False

readings_between_high_triggers=[]
readings_between_low_triggers=[]

found_first_up_trigger=False
found_first_down_trigger=False
currently_up = times_and_readings[0][1] == 1

readings_since_last_up_trigger=0
readings_since_last_down_trigger=0

readings_spent_high=[]
readings_spent_low=[]

print(f"len of this thing: {len(times_and_readings)}")
for time_and_reading in times_and_readings:
	reading=time_and_reading[1]
	readings_since_last_up_trigger+=1
	readings_since_last_down_trigger+=1
	reading_is_up = reading == "1"
	if currently_up:
		if not reading_is_up:
			if not found_first_down_trigger:
				found_first_down_trigger=True
			else:
				#we have found a down trigger and should record this one.
				readings_between_low_triggers.append(readings_since_last_down_trigger)
				readings_spent_high.append(readings_since_last_up_trigger)
			readings_since_last_down_trigger = 0
	else:
		if reading_is_up:
			if not found_first_up_trigger:
				found_first_up_trigger=True
			else:
				# we have found an up trigger and shoudl record this one.
				readings_between_high_triggers.append(readings_since_last_up_trigger)
				readings_spent_low.append(readings_since_last_down_trigger)
			readings_since_last_up_trigger = 0
	currently_up = reading_is_up

num_readings_to_number_of_high_triggers={}
for num_readings in readings_between_high_triggers:
	if not num_readings in num_readings_to_number_of_high_triggers:
		num_readings_to_number_of_high_triggers[num_readings]=0
	num_readings_to_number_of_high_triggers[num_readings]=num_readings_to_number_of_high_triggers[num_readings]+1

print("num_readings_to_number_of_high_triggers:")
print(json.dumps(num_readings_to_number_of_high_triggers, indent=4, sort_keys=True))

num_readings_to_number_of_low_triggers={}
for num_readings in readings_between_low_triggers:
	if not num_readings in num_readings_to_number_of_low_triggers:
		num_readings_to_number_of_low_triggers[num_readings]=0
	num_readings_to_number_of_low_triggers[num_readings]=num_readings_to_number_of_low_triggers[num_readings]+1



print("num_readings_to_number_of_low_triggers:")
print(json.dumps(num_readings_to_number_of_low_triggers, indent=4, sort_keys=True))



num_readings_spent_high_to_count={}
for reading_high in readings_spent_high:
	if reading_high not in num_readings_spent_high_to_count:
		num_readings_spent_high_to_count[reading_high]=0
	num_readings_spent_high_to_count[reading_high]=num_readings_spent_high_to_count[reading_high]+1
print("num_readings_spent_high_to_count:")
print(json.dumps(num_readings_spent_high_to_count, indent=4, sort_keys=True))


num_readings_spent_low_to_count={}
for reading_low in readings_spent_low:
	if reading_low not in num_readings_spent_low_to_count:
		num_readings_spent_low_to_count[reading_low]=0
	num_readings_spent_low_to_count[reading_low]=num_readings_spent_low_to_count[reading_low]+1
print("num_readings_spent_low_to_count:")
print(json.dumps(num_readings_spent_low_to_count, indent=4, sort_keys=True))

print("all done!")

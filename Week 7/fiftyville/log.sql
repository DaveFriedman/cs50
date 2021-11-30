-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Exploratory:
.schema

select description
from crime_scene_reports
where day=28 and month=7 and year=2020;

select name, transcript
from interviews
where year=2020 and month=7 and day=28;

-- Ruth:
select year, month, day, hour, minute, activity license_plate
from courthouse_security_logs
where year=2020 and month=7 and day=28;

select name from people
inner join courthouse_security_logs
    on people.license_plate = courthouse_security_logs.license_plate
where year=2020 and month=7 and day=28;

-- Eugene:
select year, month, day, account_number, atm_location, transaction_type, amount
from atm_transactions
where year=2020 and month=7 and day=28 and atm_location="Fifer Street";

select name from people
inner join bank_accounts on people.id = bank_accounts.person_id
inner join atm_transactions
    on bank_accounts.account_number = atm_transactions.account_number
where year=2020 and month=7 and day=28 and atm_location="Fifer Street";

-- Raymond:
select year, month, day, duration, caller, receiver
from phone_calls
where year=2020 and month=7 and day=28;

select name
from people
inner join phone_calls on people.phone_number = phone_calls.caller
where year=2020 and month=7 and day=28 and duration < 120;

select id, full_name from airports;

select year, month, day, hour, destination_airport_id
from flights
inner join airports on flights.origin_airport_id = airports.id
where year=2020 and month=7 and day=29
    and origin_airport_id=8;


-- The THIEF escaped to CITY (many iterations of this query):
select name, airports.city
from people
inner join phone_calls on people.phone_number = phone_calls.caller
inner join courthouse_security_logs
    on people.license_plate = courthouse_security_logs.license_plate
inner join bank_accounts on people.id = bank_accounts.person_id
inner join atm_transactions
    on bank_accounts.account_number = atm_transactions.account_number
inner join passengers on people.passport_number = passengers.passport_number
inner join flights on passengers.flight_id = flights.id
inner join airports on flights.destination_airport_id=airports.id
where
    phone_calls.year=2020
    and phone_calls.month=7
    and phone_calls.day=28
    and phone_calls.duration < 60
    and courthouse_security_logs.year=2020
    and courthouse_security_logs.month=7
    and courthouse_security_logs.day=28
    and courthouse_security_logs.hour=10
    and courthouse_security_logs.minute > 15
    and courthouse_security_logs.minute < 25
    and courthouse_security_logs.activity="exit"
    and atm_transactions.year=2020
    and atm_transactions.month=7
    and atm_transactions.day=28
    and atm_location="Fifer Street"
    and atm_transactions.transaction_type="withdraw"
    and flights.year=2020
    and flights.month=7
    and flights.day=29
    and flights.hour < 9
    and flights.origin_airport_id=8;


-- The ACCOMPLICE:
select name, phone_number
from people
where name="Ernest";

select name from people
inner join phone_calls on people.phone_number=phone_calls.receiver
where phone_calls.year=2020 and phone_calls.month=7 and phone_calls.day=28
    and phone_calls.duration<60
    and phone_calls.caller="(367) 555-5533";
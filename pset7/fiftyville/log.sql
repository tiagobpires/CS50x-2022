-- Keep a log of any SQL queries you execute as you solve the mystery.

-- I found the description of the crime scene reports
-- based on information provided by the problem
SELECT description FROM crime_scene_reports
WHERE year = 2020 AND month = 7 AND day =  28 AND street = 'Chamberlin Street';
-- description:Theft of the CS50 duck took place at 10:15am at the Chamberlin Street courthouse. 
-- Interviews were conducted today with three witnesses who were present at the time
-- — each of their interview transcripts mentions the courthouse.

-- I got the transcripts of the interviews that took place that day and that mentioned courthouse
SELECT name, transcript FROM interviews 
WHERE year = 2020 AND month = 7 AND day =  28 AND transcript LIKE '%courthouse%';
-- name | transcript
-- Ruth | Sometime within ten minutes of the theft, I saw the thief get into a car in the courthouse 
--       parking lot and drive away. If you have security footage from the courthouse parking lot, 
--       you might want to look for cars that left the parking lot in that time frame.
-- Eugene | I don't know the thief's name, but it was someone I recognized. Earlier this morning, 
--         before I arrived at the courthouse, I was walking by the ATM on Fifer Street and saw the 
--         thief there withdrawing some money.
-- Raymond | As the thief was leaving the courthouse, they called someone who talked to them 
--          for less than a minute. In the call, I heard the thief say that they were planning 
--          to take the earliest flight out of Fiftyville tomorrow. The thief then asked the 
--          person on the other end of the phone to purchase the flight ticket.

-- Based on Ruth's testimony, I looked for information about the car that the thief used
-- in the courthouse parking lot, until 10:25 (10 minutes after the theft)
SELECT license_plate FROM courthouse_security_logs
WHERE year = 2020 AND month = 7 AND day =  28 AND hour = 10 
AND activity = 'exit' AND minute BETWEEN 15 AND 25;

-- People who have these cards:
SELECT name FROM people
WHERE license_plate IN (
SELECT license_plate FROM courthouse_security_logs
WHERE year = 2020 AND month = 7 AND day =  28 AND hour = 10 
AND activity = 'exit' AND minute BETWEEN 15 AND 25);

-- Based on Raymond's testimony, I looked at the calls made after the theft and with less
-- one minute, looking for the name among the suspects who have signs that were in placel
SELECT * FROM people
WHERE license_plate IN (
SELECT license_plate FROM courthouse_security_logs
WHERE year = 2020 AND month = 7 AND day =  28 AND hour = 10 
AND activity = 'exit' AND minute BETWEEN 15 AND 25)
AND phone_number IN (
SELECT caller FROM phone_calls
WHERE year = 2020 AND month = 7 AND day =  28 AND duration <= 60);

-- id | name | phone_number | passport_number | license_plate
-- 398010 | Roger | (130) 555-0289 | 1695452385 | G412CB7
-- 514354 | Russell | (770) 555-1861 | 3592750733 | 322W7JE
-- 560886 | Evelyn | (499) 555-9472 | 8294398571 | 0NTHK55
-- 686048 | Ernest | (367) 555-5533 | 5773159633 | 94KL13X

-- Now, we have 4 suspects and I'm going to find who they called
SELECT caller, receiver FROM phone_calls
WHERE year = 2020 AND month = 7 AND day =  28 AND duration <= 60
AND caller IN(
SELECT phone_number FROM people
WHERE license_plate IN (
SELECT license_plate FROM courthouse_security_logs
WHERE year = 2020 AND month = 7 AND day =  28 AND hour = 10 
AND activity = 'exit' AND minute BETWEEN 15 AND 25)
AND phone_number IN (
SELECT caller FROM phone_calls
WHERE year = 2020 AND month = 7 AND day =  28 AND duration <= 60)
);

-- caller | receiver
-- (130) 555-0289 | (996) 555-8899   -> Who Roger called
-- (499) 555-9472 | (892) 555-8872   -> Who Evelyn called
-- (367) 555-5533 | (375) 555-8161   -> Who Ernest called
-- (499) 555-9472 | (717) 555-1342   -> Who Evelyn called
-- (770) 555-1861 | (725) 555-3243   -> Who Russell called

-- Now I'm going to find who they called
SELECT * FROM people
WHERE phone_number = '(996) 555-8899' OR phone_number = '(892) 555-8872'
OR phone_number = '(375) 555-8161' OR phone_number = '(717) 555-1342'
OR phone_number = '(725) 555-3243';

-- id | name | phone_number | passport_number | license_plate
-- 251693 | Larry | (892) 555-8872 | 2312901747 | O268ZZ0     -> Evelyn 
-- 567218 | Jack | (996) 555-8899 | 9029462229 | 52R0Y8U      -> Roger 
-- 626361 | Melissa | (717) 555-1342 | 7834357192 |           -> Evelyn 
-- 847116 | Philip | (725) 555-3243 | 3391710505 | GW362R6    -> Russell 
-- 864400 | Berthold | (375) 555-8161 |  | 4V16VO0            -> Ernest 

-- Eugene's testimony does not lead anywhere, since in the
-- table people does not have account number

-- I found the flight that leaves Fiftyville on the 29th as soon as possible
SELECT flights.id FROM flights
JOIN airports ON airports.id = flights.origin_airport_id
WHERE year = 2020 AND month = 7 AND day =  29 AND city = 'Fiftyville'
ORDER BY hour, minute LIMIT 1;
-- I found that the flight is the one with id 36

-- I found which of the suspects were on that flight
SELECT name FROM people
JOIN passengers ON passengers.passport_number = people.passport_number
JOIN flights ON flights.id = passengers.flight_id
WHERE flights.id = 36 AND passengers.passport_number IN (
SELECT passport_number FROM people
WHERE name = 'Roger' OR name = 'Russell' 
OR name = 'Evelyn' OR name = 'Ernest');
-- name
-- Roger
-- Ernest
-- Evelyn

-- Now I checked if each of the accomplices was on the flight too:
SELECT name FROM people
JOIN passengers ON passengers.passport_number = people.passport_number
JOIN flights ON flights.id = passengers.flight_id
WHERE flights.id = 36 AND passengers.passport_number IN (
SELECT passport_number FROM people
WHERE name = 'Larry' OR name = 'Jack' OR name = 'Melissa'
OR name = 'Philip' OR name = 'Berthold');

-- this action does not result in anything, therefore, Berthold, who is not with his
-- passport in the database, is our accomplice, and, checking past results,
-- I found that who called him was Ernerst, who is our thief

-- Now I will find the destination of the flight
SELECT city FROM airports
WHERE id = (
SELECT destination_airport_id FROM flights
JOIN airports ON airports.id = flights.origin_airport_id 
WHERE flights.id = 36);
-- city
-- London
/*******************************************************************/
-- Author: Brendan Jennings
-- File Name: tables.sql
-- Purpose: This file creates the sql tables  for the database 
-- 	application.
-- This file can be entered into the oracle database with the command
-- "@tables".
/*******************************************************************/

create table inventory
(
vehicle_id int PRIMARY KEY,
office_id int CONSTRAINT offices_fkey REFERENCES offices,
model_no varchar(255) CONSTRAINT models_fkey REFERENCES models,
mileage int,
plate_no varchar(8),
year int
);

create table offices
(
office_id int PRIMARY KEY,
address varchar(255),
phone_no int,
managers_contact_info varchar(255)
);

create table models
(
model_no varchar(255) PRIMARY KEY,
max_occupants int,
vehicle_type varchar(12),
description varchar(255),
cargo_capacity varchar(12),
engine_type varchar(38)
);

create table customers
(
customer_id int PRIMARY KEY,
name varchar(255),
phone_no int,
address varchar(255)
);

create table reservations
(
customer_id int CONSTRAINT customers_fkey REFERENCES customers,
vehicle_id int CONSTRAINT inventory_fkey REFERENCES inventory,
model_no CONSTRAINT model_fkey REFERENCES models,
start_date DATE,
end_date DATE,
office_id int CONSTRAINT office_fkey REFERENCES offices,
PRIMARY KEY(customer_id, vehicle_id, model_no, start_date, end_date, office_id)
);


# Brendan Jennings
# File: Sample data for csci 370 final Project
# To enter this data in sql, type @data.

insert into offices
values(123, '101 wakesiah avenue', 2507546811, 'manager@office123.com');

insert into models
values('mazda3', 5, 'sedan', '4 door grey sedan', '200 litres', '4 litre 6 cylinder');

insert into inventory
values(45453, 123, 'mazda3', 54000, 'HITNRUN', 2007);

insert into customers
values(12345, 'Hank Aaron', 2546673755, '18 vialoux drive');

insert into reservations
values(12345, 45453, 'mazda3', to_date('11-04-2015','dd-mm-yyyy'), to_date('13-04-2015', 'dd-mm-yyyy'), 123);

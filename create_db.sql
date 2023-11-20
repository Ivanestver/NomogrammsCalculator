create database if not exists nomogramms_db;
use nomogramms_db;
drop table if exists template;
create table if not exists template
(
	template_id varchar(100) not null primary key unique,
    class_id varchar(100) not null
);

drop table if exists property;
create table if not exists property
(
	property_id varchar(100) not null primary key unique,
    property_name varchar(1000) not null default "",
    property_value varchar(100) not null default ""
);

drop table if exists template_property;
create table if not exists template_property
(
	template_id varchar(100) not null,
    property_id varchar(100) not null,
    property_value varchar(1000) not null default "",
    primary key (template_id, property_id),
    foreign key (template_id) references template (template_id),
    foreign key (property_id) references property (property_id)
);

drop table if exists template_template;
create table if not exists template_template
(
	master_id varchar(100) not null,
    sub_id varchar(100) not null,
    extra varchar(1000) not null default "",
    primary key (master_id, sub_id),
    foreign key (master_id) references template(template_id),
    foreign key (sub_id) references template(template_id)
);

drop table if exists nets;
create table if not exists nets
(
	net_id varchar(100) not null primary key unique,
	net_name varchar(100) not null default "",
    net_file varchar(1000) not null default ""
);

drop table if exists measure;
create table if not exists measure
(
	measure_id varchar(100) unique primary key,
    measure_name varchar(100)
);

drop table if exists unit;
create table if not exists unit
(
	unit_id varchar(100) unique primary key,
    unit_name varchar(100)
);

drop table if exists measure_unit;
create table if not exists measure_unit
(
	measure_unit_id varchar(100) unique primary key,
    measure_id varchar(100),
    unit_id varchar(100),
    measure_unit_name varchar(1000),
    foreign key (measure_id) references measure(measure_id),
    foreign key (unit_id) references unit(unit_id)
);

drop table if exists template_measure_unit_input;
create table if not exists template_measure_unit_input
(
	template_id varchar(100),
    measure_unit_id varchar(100),
    foreign key (template_id) references template(template_id),
    foreign key (measure_unit_id) references measure_unit(measure_unit_id)
);

drop table if exists template_measure_unit_output;
create table if not exists template_measure_unit_output
(
	template_id varchar(100),
    measure_unit_id varchar(100),
    foreign key (template_id) references template(template_id),
    foreign key (measure_unit_id) references measure_unit(measure_unit_id)
);

insert into property values
('8D7A1E11-C43F-4C67-BADB-888AA3F334A9', 'dbobject_name', 'Название'),
('581060F1-2BD0-43AA-9C80-0E9107068890', 'nomogramm_bypassRule', 'Правило обхода'),
('ED6368E7-897B-4951-9705-6E7B74FD359D', 'nomogramm_bypassRule_List', 'Список правил обхода номограмм'),
('8F8F8660-6AE1-4C79-8B96-0832903C5772', 'measure_value', 'Единица измерения'),
('C3AA13ED-4620-4016-A106-7AAE653CBFD1', 'data_type_with_unit', 'Тип данных с единицей измерения'),
('E67ABF23-9E60-49A4-BB5E-DC44F50B649E', 'data_output_type_with_unit', 'Тип выходных данных с единицей измерения');
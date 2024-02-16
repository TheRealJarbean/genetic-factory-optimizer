-- SQLite
-- Used by updateDatabaseFromWiki.py
DROP TABLE IF EXISTS resources;
DROP TABLE IF EXISTS constructor;
DROP TABLE IF EXISTS assembler;
DROP TABLE IF EXISTS manufacturer;

CREATE TABLE resources (
    id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
    name VARCHAR(255) NOT NULL UNIQUE
);

CREATE TABLE constructor (
    id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
    input1_id INTEGER NOT NULL,
    input1_rate REAL NOT NULL,
    output_id INTEGER NOT NULL,
    output_rate REAL NOT NULL,
    FOREIGN KEY (input1_id) REFERENCES resources(id),
    FOREIGN KEY (output_id) REFERENCES resources(id)
);

CREATE TABLE assembler (
    id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
    input1_id INTEGER NOT NULL,
    input1_rate REAL NOT NULL,
    input2_id INTEGER NOT NULL,
    input2_rate REAL NOT NULL,
    output_id INTEGER NOT NULL,
    output_rate REAL NOT NULL,
    FOREIGN KEY (input1_id) REFERENCES resources(id),
    FOREIGN KEY (input2_id) REFERENCES resources(id),
    FOREIGN KEY (output_id) REFERENCES resources(id)
);

CREATE TABLE manufacturer (
    id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
    input1_id INTEGER NOT NULL,
    input1_rate REAL NOT NULL,
    input2_id INTEGER NOT NULL,
    input2_rate REAL NOT NULL,
    input3_id INTEGER NOT NULL,
    input3_rate REAL NOT NULL,
    input4_id INTEGER,
    input4_rate REAL,
    output_id INTEGER NOT NULL,
    output_rate REAL NOT NULL,
    FOREIGN KEY (input1_id) REFERENCES resources(id),
    FOREIGN KEY (input2_id) REFERENCES resources(id),
    FOREIGN KEY (input3_id) REFERENCES resources(id),
    FOREIGN KEY (input4_id) REFERENCES resources(id),
    FOREIGN KEY (output_id) REFERENCES resources(id)
);
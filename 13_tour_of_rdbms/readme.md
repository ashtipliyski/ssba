# Tour of Relational Databases

## Building Postgres from scratch

Get source code

```bash
git clone https://github.com/postgres/postgres.git
```

comment out configuration options that specify `-O2` optimisations that might make it more difficult to interpret the code during debugging.

Enable debugging during configuration, using 

```bash
./configure --enable-debug
make
```
and install

```bash
install
```

Post installation steps

- add `/usr/local/pgsql/bin` to path
- create postgress user `$ adduser posgres`




## Instructions

- install readline (for ubuntu VM): `sudo apt-get install libreadline-dev -y`
- install zlib (for ubuntu VM): `sudo apt-get install -y zlib1g-dev`
- install bison (for ubuntu VM): `sudo apt-get install -y bison`
- install flex (for ubuntu vm): `sudo apt-get install -y flex`
- install stow (for ubuntu vm): `sudo apt-get install -y stow`
- get source from git `git clone https://github.com/postgres/postgres.git`
- modify config script to disable O2 optimisations
  - replace occurrences of `CFLAGS="-g -O2` to `CFLAGS="-g"`
  - replace occurrences of `CFLAGS="-O2` to `CFLAGS=""`
  - replace occurrences of `CXXFLAGS="-g -O2` to `CXXFLAGS="-g"`
  - replace occurrences of `CXXFLAGS="-O2` to `CXXFLAGS=""`
  - replace occurrences of `BITCODE_CFLAGS="-O2 $BITCODE_CFLAGS` to `BITCODE_CFLAGS="-O2 $BITCODE_CFLAGS`
  - replace occurrences of `BITCODE_CXXFLAGS="-O2 $BITCODE_CXXFLAGS` to `BITCODE_CXXFLAGS="-O2 $BITCODE_CXXFLAGS`
- configure with debug options `./configure --enable-debug`
- make `make`
- install with `sudo make install`
- go to contrib, make and install it `cd contrib; make; sudo make install;-`
- go to contrib, make and install it `cd xml2; make; sudo make install; cd ../..`
- "stow" the `pqsql` binary `stow pgsql` (Note: I am not sure what this does)
- create a user for postgres `sudo adduser postgres`
- login as `su` to configure the installation directory: `sudo su -`
- configure installation

```bash
mkdir -p /usr/local/pgsql/data
chown postgres /usr/local/pgsql/data
su - postgres
/usr/local/pgsql/bin/initdb -D /usr/local/pgsql/data -E UNICODE --locale=C
/usr/local/pgsql/bin/pg_ctl -D /usr/local/pgsql/data -l /home/postgres/logfile start
```
- add pgsql install folder to `~/.profile` with `cat PATH="/usr/local/pgsql/bin:$PATH" >> ~/.profile`

Note that it is best to use the `postgres` user to interact with the database

- get test dataset `wget https://files.grouplens.org/datasets/movielens/ml-20m.zip`


## Import dataset

- login as postgres user `sudo su - postgres`
- start psql `psql`
- create movies database `create table movies (id Serial,title varchar(255),genres varchar(255),primary key (id));`
- import movies `copy movies from '/home/postgres/ml-20m/movies.csv' delimiter ',' csv header;`
- 

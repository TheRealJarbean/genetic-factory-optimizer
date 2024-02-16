import sqlite3
import requests
from bs4 import BeautifulSoup

def scrape_recipes(building_url):
	soup = BeautifulSoup(requests.get(building_url).content, 'html5lib')

	# Currently (fortunately) the Recipes table is returned from this query.
	# It may need tuning if the site layout changes but so
	# would every other query.
	# Row with column labels is removed
	recipe_rows = soup.find('table', attrs={'class':'wikitable'}).find_all('tr')[1:]

	recipes = []
	for row in recipe_rows:
		# Span elements include the name and input/output rate
		# of the resources involved the recipe. These appear next
		# to each other in name/rate pairs in the following list.
		# The output will be the name/rate pair.
		resources = row.find_all('span')

		# Remove all HTML tag information
		resources = [resource.text for resource in resources]

		# The first item in the list is sometimes 'alt' and is removed.
		if resources[0] == 'alt':
			resources = resources[1:]

		# Convert flat list to two-dimensional list
		resources = [[resources[i], resources[i + 1]] for i in range(0, len(resources), 2)]

		# Trim leading non-breaking space '\xa0' from resource name
		# Trim '/min' from rate string and convert to int
		recipe = [[resource[0][1:], float(resource[1][:-4])] for resource in resources]
		recipes.append(recipe)

	return recipes

def create_insert_recipe_query(recipe):
	num_machine_inputs = len(recipe) - 1

	# Manufacturer can have 3 or 4 inputs
	tables = ['constructor', 'assembler', 'manufacturer', 'manufacturer']
	table = tables[num_machine_inputs - 1]

	query = f"INSERT INTO {table} ("
	for i in range(1, num_machine_inputs + 1):
		query += f"input{i}_id, input{i}_rate, "
	query += "output_id, output_rate) VALUES ("
	for resource, rate in recipe:
		query += f"(SELECT id FROM resources WHERE name = {resource}), {rate}, "

	# Remove extra ', ' added by previous loop
	# and add final closing parenthesis.
	query = query[:-2] + ")"

	return query

def create_insert_resource_query(resource_name):
	query = f"INSERT OR IGNORE INTO resources (name) VALUES ({resource_name})"
	return query

conn = sqlite3.connect('data/recipes.sqlite')
cursor = conn.cursor()

# Run preconfigured SQLite script to delete
# and recreate all database tables
sqlfile = open('scripts/initializeDatabase.sql', 'r')
initialize_database_query = sqlfile.read()
sqlfile.close()

# Only one command can be run at a time in sqlite3
sqlCommands = initialize_database_query.split(';')
for command in sqlCommands:
	cursor.execute(command)

# UPDATE CONSTRUCTOR RECIPES
for recipe in scrape_recipes('https://satisfactory.fandom.com/wiki/Constructor'):
	# Double quotes added for SQL query formatting, convert rate to int
	recipe = [['\"' + resource[0] + '\"', resource[1]] for resource in recipe]
	for resource, _ in recipe:
		query = create_insert_resource_query(resource)
		cursor.execute(query)
	query = create_insert_recipe_query(recipe)
	cursor.execute(query)

# UPDATE ASSEMBLER RECIPES
for recipe in scrape_recipes('https://satisfactory.fandom.com/wiki/Assembler'):
	# Double quotes added for SQL query formatting, convert rate to int
	recipe = [['\"' + resource[0] + '\"', resource[1]] for resource in recipe]
	for resource, _ in recipe:
		query = create_insert_resource_query(resource)
		cursor.execute(query)
	query = create_insert_recipe_query(recipe)
	cursor.execute(query)

# UPDATE MANUFACTURER RECIPES
for recipe in scrape_recipes('https://satisfactory.fandom.com/wiki/Manufacturer'):
	# Double quotes added for SQL query formatting, convert rate to int
	recipe = [['\"' + resource[0] + '\"', resource[1]] for resource in recipe]
	for resource, _ in recipe:
		query = create_insert_resource_query(resource)
		cursor.execute(query)
	query = create_insert_recipe_query(recipe)
	cursor.execute(query)

conn.commit()
conn.close()
import itertools

constructors = [
	{"_default": "create_default"},
	{"_copy": "create_copy"},
	{"_copy_move": "create_copy_move"},
	{"_move": "create_move"},
]

testcases = [
  {
    "name": "smoke",
    "module_count": 0,
    "implemented":
    {
        "_default": "create",
    },
  },
   {
    "name": "default",
    "module_count": 1,
    "implemented":
    {
        "_default": "create",
    },
  },
]

# for length in range(1, len(constructors) + 1):
# for length in range(1, 1):
# 	for combination in itertools.combinations(constructors, length):
# 		for key, value in combination:
# 			testcases.append({
# 				"name": "only" + key,
# 				"module_count": 1,
# 				"implemented": {key: value}
# 			})
import shutil
import os
import platform
import subprocess
from mako.template import Template
from testcases import testcases as cases

TEMPLATE_FOLDER = "templates"

MODULE_TEMPLATE = {
  "in": "example.cpp.tmpl",
  "out": "example{counter}.cpp",
}

TEMPLATIZED_FILES = [
  {
    "in": "CMakeLists.txt.tmpl",
    "out": "CMakeLists.txt",
  },
  {
    "in": "main.cpp.tmpl",
    "out": "main.cpp",
  },
  {
    "in": "base.hpp.tmpl",
    "out": "base.hpp",
  },
  {
    "in": "params.txt.tmpl",
    "out": "params.txt",
  },
]


def cleanup(path, cases):
  """
  Clean up the cases folders recursively
  """
  for case in cases:
    case_dir = case['name']
    if os.path.exists(os.path.join(path, case_dir)) and os.path.isdir(os.path.join(path, case_dir)):
      shutil.rmtree(os.path.join(path, case_dir))


def generate_modules(params):
  for counter in params['module_range']:
    template = Template(filename=os.path.join(params['templates_dir'], MODULE_TEMPLATE['in']))
    with open(os.path.join(params['case_dir'], MODULE_TEMPLATE['out'].format(counter=counter)), 'w') as outputfile:
      outputfile.write(template.render(unique=counter, **params))


def generate_files(params):
  generate_modules(params=params)
  for template_info in TEMPLATIZED_FILES:
    template = Template(filename=os.path.join(params['templates_dir'], template_info['in']))
    with open(os.path.join(params['case_dir'], template_info['out']), 'w') as outputfile:
      outputfile.write(template.render(**params))


def get_library_name(name):
  system = platform.system()
  if system == 'Linux':
    return 'lib' + name.replace('.cpp', '.so')
  if system == 'darwin':
    return 'lib' + name.replace('.cpp', '.dylib')


def generate_params(path, case):
  params = {}
  params['unittest_dir'] = path
  params['case_name'] = case['name']
  params['implemented'] = case['implemented']
  params['templates_dir'] = os.path.join(params['unittest_dir'], TEMPLATE_FOLDER)
  params['project_dir'] = os.path.split(params['unittest_dir'])[0]
  params['case_dir'] = os.path.join(params['unittest_dir'], case['name'])
  params['module_range'] = range(1, case['module_count'] + 1)
  params['module_sources'] = []
  params['modules'] = []
  for counter in params['module_range']:
    source_name = MODULE_TEMPLATE['out'].format(counter=counter)
    params['module_sources'].append(source_name)
    params['modules'].append(os.path.join(params['case_dir'], get_library_name(source_name)))

  return params


def generate_cases(path, cases):
  """
  Generate all cases
  """
  for case in cases:
    case_dir = case['name']
    os.mkdir(os.path.join(path, case_dir))
    params = generate_params(path=path, case=case)
    generate_files(params=params)


def check_cases(path, cases):
  """
  Execute the cases
  """
  for case in cases:
    case_dir = os.path.join(path, case['name'])
    subprocess.check_call(["cmake", "."], cwd=case_dir)
    subprocess.check_call(["make"], cwd=case_dir)
    subprocess.check_call([os.path.join(case_dir, case['name'])])


if __name__ == "__main__":
  path = os.path.dirname(os.path.realpath(__file__))
  cleanup(path=path, cases=cases)
  generate_cases(path=path, cases=cases)
  check_cases(path=path, cases=cases)

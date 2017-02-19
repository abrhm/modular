import shutil
import os
import subprocess
import progressbar
from mako.template import Template

testcases = {
  "smoke" : {
    "compile": True,
  },
}


def cleanup(cases):
  for case in cases:
    if os.path.exists(case) and os.path.isdir(case):
      shutil.rmtree(case)


def generate_cmake(case):
  cmake_template = Template(filename="CMakeLists.txt.tmpl")
  project_root = os.path.split(os.getcwd())[0]
  with open(os.path.join(case, "CMakeLists.txt"), "w") as outputfile:
    outputfile.write(cmake_template.render(case=case, project_root=project_root))


def generate_main(case):
  main_template = Template(filename="main.cpp.tmpl")
  with open(os.path.join(case, "main.cpp"), "w") as outputfile:
    outputfile.write(main_template.render(case=case))


def generate_cases(cases):
  bar = progressbar.ProgressBar()
  for case, params in bar(cases.items()):
    os.mkdir(case)
    generate_cmake(case=case)
    generate_main(case=case)


def check_cases(cases):
  for case, params in cases.items():
    subprocess.check_call(["cmake", "."], cwd=os.path.join(os.getcwd(), case))
    subprocess.check_call(["make"], cwd=os.path.join(os.getcwd(), case))
    subprocess.check_call([os.path.join(os.getcwd(), case, case)])


if __name__ == "__main__":
  cleanup(cases=testcases)
  generate_cases(cases=testcases)
  check_cases(cases=testcases)


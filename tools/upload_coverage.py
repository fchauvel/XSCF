#! /usr/bin/python


import io
import os
import subprocess
import requests
import argparse


class Record:
    
    def __init__(self, properties):
        self._properties = [each for each in properties]
        
    def accept(self, visitor):
        visitor.visit_record(self)
        
    @property
    def properties(self):
        return self._properties

    def __contains__(self, key):
        return any(property.key == key for property in self._properties)

    def __getitem__(self, key):
        candidates = [property for property in self._properties if property.key == key]
        if len(candidates) == 1:
            return candidates[0]
        return candidates
    
    @property
    def keys(self):
        return self._properties.keys()

    @property
    def branch_coverage(self):
        return 100 * (self["BRH"].as_integer / self["BRF"].as_integer)

    @property
    def function_coverage(self):
        return 100 * (self["FNH"].as_integer / self["FNF"].as_integer)

    @property
    def line_coverage(self):
        return 100 * (self["LH"].as_integer / self["LF"].as_integer)
    
    
    
class Property:

    KEY_SEPARATOR = ":"
    VALUE_SEPARATOR = ","

    @staticmethod
    def from_text(text):
        key, entry = text.split(Property.KEY_SEPARATOR)
        values = entry.split(Property.VALUE_SEPARATOR)
        return Property(key, values)

    def __init__(self, key, values):
        self._key = key
        self._values = values

    @property
    def key(self):
        return self._key

    @property
    def values(self):
        return self._values

    @property
    def as_integer(self):
        return int(self._values[0])

    @property
    def as_text(self):
        return "".join(each for each in self._values)
    
    

class InfoFile:

    RECORD_SEPARATOR = "end_of_record"

    @staticmethod
    def from_file(file_name):
        with open(file_name, "r") as source:
            records = []
            properties = []
            for each_line in source.readlines():                
                each_clean_line = each_line.strip()
                if each_clean_line == InfoFile.RECORD_SEPARATOR:
                    records.append(Record(properties))
                    properties = []
                else:
                    properties.append(Property.from_text(each_clean_line))
            return InfoFile(records)
    

    def __init__(self, records):
        self._records = records

    def accept(self, visitor):
        visitor.visit_info_file(self)
        
    @property
    def records(self):
        return self._records

    @property
    def size(self):
        return len(self._records)


    
class CodacityJsonFormatter:

    def __init__(self, out):
        self._out = out
        self._total_line_count = 0
        self._total_hit_count = 0
    
    def visit_info_file(self, info_file):
        self._print("{{")
        self._print("\"fileReports\": [")
        for index, any_record in enumerate(info_file.records):
            if "SF" in any_record and "XSCF" in any_record["SF"].as_text:
                if index > 1:
                    self._print(", ")
                any_record.accept(self)

        self._print("],")
        self._print("\"total\": {total}, ".format(total=self._overall_coverage))
        self._print("\"codeLines\": {lines}".format(lines=self._total_line_count))
        self._print("}}")
        
    @property
    def _overall_coverage(self):
        return int(100 * self._total_hit_count / self._total_line_count)

    def visit_record(self, record):
        self._print("{{")
        self._print("\"filename\": \"{source_file}\",", source_file=self._local_path(record))
        self._print("\"total\": {total},", total=record["LF"].as_integer)
        self._total_line_count += record["LF"].as_integer
        self._print("\"codeLines\": {hit},", hit=record["LH"].as_integer)
        self._total_hit_count += record["LH"].as_integer
        self._print("\"coverage\": {{")
        for index, each_line in enumerate(record["DA"]):
            line_number, hit_count = each_line.values
            self._print("\"{line_number}\": {hit_count}",
                        line_number=line_number,
                        hit_count=hit_count);
            if index < len(record["DA"])-1:
                self._print(", ")
        self._print("}}")
        self._print("}}")

    def _local_path(self, record):
        _, resource = record["SF"].as_text.split("XSCF/")
        return resource

    def _print(self, template, **values):
        text = template.format(**values)
        print(text, file=self._out, end="")


class CodacyService:

    DEFAULT_URL = "https://api.codacy.com/2.0/{user}/{repository}/commit/{commit}/coverage/cpp"
    
    def __init__(self, user, repository, commit, project_token, codacy_url=None):
        self._user = user
        self._repository = repository
        self._commit = commit
        self._project_token = project_token
        self._codacy_url = codacy_url or self.DEFAULT_URL
        
    def send_coverage(self, coverage):
        response = requests.post(
            self._url,
            headers = {
                "Accept": "application/json",
                "project_token": project_token
            },
            data = self._to_json(coverage)
        )
        return response.status_code, response.reason

    @property
    def _url(self):
        return self._codacy_url.format(
            user=self._user,
            repository=self._repository,
            commit=self._commit)

    def _to_json(self, coverage):
        json = io.StringIO()
        formatter = CodacityJsonFormatter(json)
        coverage.accept(formatter)
        return json.getvalue()


class GcovUploader:

    def __init__(self, codacy):
        self._codacy = codacy
        
    def upload_coverage(self, source):
        gcov_data = InfoFile.from_file(source)
        print("GCOV data read from '{file}'".format(file=source))
        status, reason = self._codacy.send_coverage(gcov_data)
        print("Uploaded", status, reason)


def default_for(variable):
    value = os.getenv(variable)
    if not value:
        error = "The environment variable '{variable}' is not defined or empty."
        raise ValueError(error.format(variable=variable))
    return value

def fetch_commit():
    commit = subprocess.check_output(["git", "log", "--format=\"%H\"", "-n", "1"]).decode("ascii")
    return commit.replace("\"", "")



if __name__ == "__main__":
    parser = argparse.ArgumentParser(prog="CodacyGCOV")
    parser.add_argument("-u", "--user", help="your Codacy user name")
    parser.add_argument("-p", "--project", help="the name of your Codacy project")
    parser.add_argument("-i", "--input-file", required=True, help="the coverage file generated by gcov (.info)")
    
    arguments = parser.parse_args()

    user = arguments.user
    project = arguments.project
    commit = fetch_commit()
    project_token = default_for("PROJECT_TOKEN")
    
    uploader = GcovUploader(CodacyService(user, project, commit, project_token))
    uploader.upload_coverage(arguments.input_file)
    
    


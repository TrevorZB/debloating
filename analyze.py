import requests

class Analyze:
    def __init__(self, key, bloat_filename, debloat_filename, name, types, strats):
        self.bloat_filename = bloat_filename
        self.bloat_id = None
        self.bloat_resp = None
        self.bloat_detected = []

        self.debloat_filename = debloat_filename
        self.debloat_id = None
        self.debloat_resp = None
        self.debloat_detected = []

        self.name = name
        self.types = types
        self.strats = strats
        self.row = [''] * 9
        self.headers = {'x-apikey': key}
        self.base = 'https://www.virustotal.com/api/v3'
        

    def analyze(self):
        r = self._analyze(self.bloat_filename)
        if r:
            self.bloat_id = r.json()['data']['id']

        r = self._analyze(self.debloat_filename)
        if r:
            self.debloat_id = r.json()['data']['id']
    
    def _analyze(self, filename):
        f = open(filename, 'r')
        if not f:
            print(f'failed to open file: {filename}')
            return None
        r = requests.post(f'{self.base}/files', headers=self.headers, files={'file': f})
        f.close()

        if r.status_code != 200:
            print('Error in file upload request')
            return None

        return r

    def get_analysis(self):
        if not self.bloat_resp:
            self.bloat_resp = self._get_analysis(self.bloat_id)
        
        if not self.debloat_resp:
            self.debloat_resp = self._get_analysis(self.debloat_id)

        if self.bloat_resp and self.debloat_resp:
            print('Both analyses have been collected')

    def _get_analysis(self, target_id):
        r = requests.get(f'{self.base}/analyses/{target_id}', headers=self.headers)
        if r.status_code != 200:
            print('Error in analyses request')
            return None
        resp = r.json()
        if resp['data']['attributes']['status'] == 'queued':
            print('Analysis is queued')
            print('Wait ~1 minute then retry get_analysis call')
            return None
        return resp

    def get_csv_row(self):
        bloat_size = self.bloat_resp['meta']['file_info']['size']
        debloat_size = self.debloat_resp['meta']['file_info']['size']
        bloat_count = self.bloat_resp['data']['attributes']['stats']['malicious']
        debloat_count = self.debloat_resp['data']['attributes']['stats']['malicious']

        for k, v in self.bloat_resp['data']['attributes']['results'].items():
            if v['category'] == 'malicious':
                self.bloat_detected.append(k)

        for k, v in self.debloat_resp['data']['attributes']['results'].items():
            if v['category'] == 'malicious':
                self.debloat_detected.append(k)

        row = self.row

        types = ''
        for t in self.types:
            types = f'{types};{t}'
        types = types[1:]

        st = ''
        for strat in self.strats:
            st = f'{st};{strat}'
        st = st[1:]

        bloat_detected = ''
        for detec in self.bloat_detected:
            bloat_detected = f'{bloat_detected};{detec}'
        bloat_detected = bloat_detected[1:]

        debloat_detected = ''
        for detec in self.debloat_detected:
            debloat_detected = f'{debloat_detected};{detec}'
        debloat_detected = debloat_detected[1:]

        row[0] = self.name
        row[1] = types
        row[2] = st
        row[3] = str(bloat_size)
        row[4] = str(debloat_size)
        row[5] = str(bloat_count)
        row[6] = str(debloat_count)
        row[7] = bloat_detected
        row[8] = debloat_detected

        return ','.join(row)

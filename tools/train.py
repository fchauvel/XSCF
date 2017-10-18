#!/usr/bin/pyhton

from subprocess import Popen, PIPE


class Data:

    def __init__(self, observation, expected_prediction):
        self._observation = observation
        self._expected_prediction = expected_prediction

    def error_with(self, prediction):
        return (prediction - self._expected_prediction)**2

    @property
    def observation(self):
        return self._observation


class Agent:
    EXECUTABLE = "bin/dist/XCSF_0.0.1.exe"

    def __init__(self, logfile):
        self._logfile = logfile
        self._process = Popen(self.EXECUTABLE,
                              stdin=PIPE,
                              stdout=PIPE,
                              bufsize=1,
                              universal_newlines=True)
        self._receive()

    def accept_reward(self, reward):
        text = "R:%.4f\n" % reward
        self._send(text)

    def predict(self, observation):
        self._send("P:(%.2f)\n" % observation)
        return float(self._receive().strip("[]\n"))

    def _send(self, text):
        logfile.write(">>>" + text)
        self._process.stdin.write(text)

    def _receive(self):
        while True:
            line = self._process.stdout.readline()
            if line:
                logfile.write("<<<" + line)
                return line


class Coach:

    def __init__(self, training_data):
        self._training_data = training_data

    def train(self, trainee, rounds=10):
        progress = []
        for round in range(rounds):
            for each_data in self._training_data:
                prediction = trainee.predict(each_data.observation)
                money = self._reward(each_data, prediction)
                trainee.accept_reward(money)
                progress.append((each_data.error_with(prediction), money))
        return progress

    def _reward(self, data, prediction):
        return 1000. / (10 + data.error_with(prediction))


class View:

    def __init__(self, progress):
        self._data = progress

    def show_error(self):
        import matplotlib.pyplot as plt
        plt.scatter(self._inputs(), self._errors())
        plt.show()

    def show_reward(self):
        import matplotlib.pyplot as plt
        plt.scatter(self._inputs(), self._rewards())
        plt.show()

    def _inputs(self):
        return [i for i, each in enumerate(self._data)]

    def _errors(self):
        return [each[0] for i, each in enumerate(self._data)]

    def _rewards(self):
        return [each[1] for i, each in enumerate(self._data)]


if __name__ == "__main__":
    model = [ Data(x, x) for x in range(100) ]

    with open("training.log", "w") as logfile:
        agent = Agent(logfile)
        coach = Coach(model)

        progress = coach.train(agent, rounds=2)

        view = View(progress)
        print(view._inputs())
        view.show_error()

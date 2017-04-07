

BETA = .25
V = 2
THRESHOLD = 500


class Rule:

    def __init__(self, payoff, fitness, error):
        self._payoff = payoff
        self._error = error
        self._fitness = fitness

    @property
    def fitness(self):
        return self._fitness

    @property
    def error(self):
        return self._error

    @property
    def payoff(self):
        return self._payoff

    @property
    def weighted_payoff(self):
        return self.payoff * self.fitness

    

def update(rule_set, reward):
    payoffs = []
    errors = []
    accuracies = []
    fitnesses = []
    total_accuracy = 0
    for each_rule in rule_set:
        payoffs.append(each_rule.payoff + BETA * (reward - each_rule.payoff))
        errors.append(each_rule.error + BETA * abs(reward - payoffs[-1]))
        accuracies.append(1.0)
        if errors[-1] > THRESHOLD:
            accuracies[-1] = 0.1 * (errors[-1] / THRESHOLD)**-V
        total_accuracy += accuracies[-1]
        
    for index, rule in enumerate(rule_set):
        relative_accuracy = accuracies[index] / total_accuracy
        fitnesses.append(rule_set[index].fitness + BETA * (relative_accuracy - rule_set[index].fitness))
    print("{0:>10} {1:>10} {2:>10} {3:>10}".format("PAYOFF", "ERROR", "FITNESS", "W. PAYOFF"))
    for index, each_rule in enumerate(rule_set):
        print("{0:10.3f} {1:10.3f} {2:10.3f} {3:10.3f}".format(payoffs[index], errors[index], fitnesses[index],payoffs[index] * fitnesses[index]))

rules = [
    Rule(payoff=1.0, fitness=1.0, error=1.0),
    Rule(payoff=0.8, fitness=0.8, error=1.0)
]

update(rules, reward=10)

            
        

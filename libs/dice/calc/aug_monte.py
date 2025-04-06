import numpy as np
from collections import Counter
import os
import matplotlib.pyplot as plt
import multiprocessing as mp

dice_per_sim = 3
sim_count = 100000
aug_multiplicity = 1
hist_bins = np.arange(dice_per_sim, dice_per_sim * 6 + 2) - 0.5 # Create bins centered on integers

def plot(data, label):
    plt.hist(data, bins=hist_bins, label=label, density=True)

def roll_dice(count):
    return [np.random.randint(1, 7) for _ in range(count)]

def sum_sim(strength):
    values = roll_dice(dice_per_sim)
    total = sum(values)
    return total

def cut_sim(strength):
    values = sorted(roll_dice(dice_per_sim + strength))
    total = sum(values[:-strength])  # Sum all but the highest 'aug_multiplicity' values
    return total

def drop_sim(strength):
    values = sorted(roll_dice(dice_per_sim + strength))
    total = sum(values[strength:])  # Sum all but the lowest 'aug_multiplicity' values
    return total

def cut_drop_sim(strengths):
    cut_strength, drop_strength = strengths
    if (cut_strength == 0): return drop_sim(drop_strength)
    if (drop_strength == 0): return cut_sim(cut_strength)
    if (cut_strength == 0 and drop_strength == 0): return sum_sim(0)
    values = sorted(roll_dice(dice_per_sim + cut_strength + drop_strength))
    total = sum(values[drop_strength:-cut_strength])  # Sum all but the highest 'cut_strength' and lowest 'drop_strength' values
    return total

def bump_sim(strength):
    values = sorted(roll_dice(dice_per_sim), reverse=True)
    count = strength
    tot = sum(values)
    for i in values:
        if count == 0:
            return tot
        if i < 6:
            tot -= i
            tot += 6
            count -= 1
    return tot # There are not enough dice < 6 to bump

def max_sim(strength):
    values = sorted(roll_dice(dice_per_sim))
    total = sum(values[strength:])
    return total + strength * 6

class monte:
    def __init__(self, func, label=None, title=None, filename=None):
        self.label = label
        self.func = func
        self.title = title
        self.filename = filename
    
    def run(self, strength=None):
        global aug_multiplicity
        if strength is None:
            strength = aug_multiplicity
        with mp.Pool(mp.cpu_count()) as pool:
            self.results = pool.map_async(self.func, [strength] * sim_count).get()
        pool.close()
        pool.join()
        # self.results = [self.func(strength) for _ in range(sim_count)]
        return self
    
    def plot(self):
        plot(self.results, label =self.label)
        return self

def configure_hist():
    plt.figure(figsize=(10, 6))
    plt.xticks([dice_per_sim, dice_per_sim * 2, dice_per_sim * 3, dice_per_sim * 4, dice_per_sim * 5, dice_per_sim * 6])
    plt.xlabel('Sum of Rolls')
    plt.ylabel('Frequency')
    return

def run_all_sims():
    sum_m=monte(sum_sim, label="None",title="Sum Sim ("+str(dice_per_sim)+" Dice, "+str(aug_multiplicity)+" Strength)",filename=str(dice_per_sim)+"_sum_"+str(aug_multiplicity)).run()
    cut_m=monte(cut_sim, label="Cut",title="Cut Sim ("+str(dice_per_sim)+" Dice, "+str(aug_multiplicity)+" Strength)",filename=str(dice_per_sim)+"_cut_"+str(aug_multiplicity)).run()
    drop_m=monte(drop_sim, label="Drop",title="Drop Sim ("+str(dice_per_sim)+" Dice, "+str(aug_multiplicity)+" Strength)",filename=str(dice_per_sim)+"_drop_"+str(aug_multiplicity)).run()
    bump_m=monte(bump_sim, label="Bump",title="Bump Sim ("+str(dice_per_sim)+" Dice, "+str(aug_multiplicity)+" Strength)",filename=str(dice_per_sim)+"_bump_"+str(aug_multiplicity)).run()
    max_m=monte(max_sim, label="Max",title="Max Sim ("+str(dice_per_sim)+" Dice, "+str(aug_multiplicity)+" Strength)",filename=str(dice_per_sim)+"_max_"+str(aug_multiplicity)).run()
    for m in [sum_m, cut_m, drop_m, bump_m, max_m]:
        configure_hist()
        m.plot()
        plt.title(m.title)
        plt.savefig('monte/'+m.filename)
        plt.clf()
        plt.close()
    configure_hist()
    for m in [sum_m, cut_m, drop_m, bump_m, max_m]:
        m.plot()
    plt.legend()
    plt.title('Simulation of '+str(dice_per_sim)+' Dice Rolls (Augment Strength '+str(aug_multiplicity)+')')
    plt.savefig('monte/'+str(dice_per_sim)+'_monte_' + str(aug_multiplicity))
    plt.show()
    plt.close()
    plt.clf()
    return
    
def cut_drop_spec_slice(strength):
    monte(cut_sim, "Cut "+str(strength), 'Sum of '+str(dice_per_sim)+' Dice Rolls Cutting ' +str(aug_multiplicity), 'cut_monte').run(strength).plot()
    monte(drop_sim, "Drop "+str(strength), 'Sum of '+str(dice_per_sim)+' Dice Rolls Dropping ' +str(aug_multiplicity), 'drop_monte').run(strength).plot()
    return

def cut_drop_spec():
    partitions = [dice_per_sim * i // 5 for i in range(1, 6)]

    plt.figure(figsize=(10, 6))
    monte(sum_sim, "None", 'Sum of '+str(dice_per_sim)+' Dice Rolls', 'sum_monte').run().plot()
    for i in partitions:
        cut_drop_spec_slice(i)
    
    configure_hist()
    plt.title('Simulation of '+str(dice_per_sim)+' Dice Rolls, Showing Cut and Drop Strength Spectrums')
    plt.savefig('monte/'+str(dice_per_sim)+'_monte_spec')
    plt.show()
    plt.close()
    plt.clf()
    return

def drop_strength_rel():
    partitions = range(0,aug_multiplicity + 1)
    montes = map(lambda x: monte(drop_sim, label="Drop"+str(x)).run(x), partitions)
    plt.figure(figsize=(10, 6))
    means = [np.mean(m.results) for m in montes]
    plt.plot(partitions, means, marker='o')
    plt.xlabel('Strength')
    plt.ylabel('Mean Value')
    plt.title('Mean Value vs Strength for Drop Simulation ('+str(dice_per_sim)+' Dice)')
    plt.grid()
    plt.savefig('monte/'+str(dice_per_sim)+'drop_strength_rel')
    plt.show()
    plt.close()
    plt.clf()
    return

def drop_cut_ratio_rel():
    ratio_limit = 5
    ratios = [(cut_strength, drop_strength) for cut_strength in range(0, ratio_limit) 
              for drop_strength in range(cut_strength + 1, ratio_limit + 1)]
    coefficients = range(aug_multiplicity+1)
    for cut_strength, drop_strength in ratios:
        ratio_string = "C/D "+str(cut_strength) + ":" + str(drop_strength)
        partitions = [(cut_strength*c, drop_strength*c) for c in coefficients]
        montes = map(lambda x: monte(cut_drop_sim, label="Cut"+str(x[0])+" Drop"+str(x[1])).run(x), partitions)
        means = [np.mean(m.results) for m in montes]
        plt.plot(coefficients, means, marker='o', label=ratio_string)
    plt.figure(figsize=(10, 6))
    plt.xlabel('Coefficient')
    plt.ylabel('Mean Value')
    plt.title('Mean Value vs Coefficient for Cut/Drop Simulation ('+str(dice_per_sim)+' Dice)')
    plt.legend()
    plt.grid()
    plt.savefig('monte/'+str(dice_per_sim)+'cut_drop_ratio_rel')
    plt.show()
    plt.close()
    plt.clf()
    return

if __name__ == "__main__":
    if not os.path.exists('monte'):
        os.makedirs('monte')
    run_all_sims()
    # cut_drop_spec()
    # drop_strength_rel()
    # drop_cut_ratio_rel()
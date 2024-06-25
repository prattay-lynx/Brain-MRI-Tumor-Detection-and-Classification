import random
import cv2
import numpy as np

def create_initial_population(pop_size, chromlen):
    population = []
    for _ in range(pop_size):
        chromosome = [random.randint(0, 1) for _ in range(chromlen)]
        population.append(chromosome)
    return population

def decode_parameters(chromosome, min_range, max_range):
    decoded_parameters = []
    for i in range(12):
        parameter_binary = ''.join(map(str, chromosome[i * 10: (i + 1) * 10]))
        value = min_range[i] + (max_range[i] - min_range[i]) * int(parameter_binary, 2) / (2**10 - 1)
        decoded_parameters.append(value)
    return decoded_parameters

def apply_parameters(input_image, parameter, min_range_i, max_range_i):
    if len(input_image.shape) > 2:
        input_image = cv2.cvtColor(input_image, cv2.COLOR_BGR2GRAY)

    # Ensure the denominator is not zero
    denominator = max_range_i - min_range_i
    if denominator == 0:
        return input_image.astype(np.uint8)

    # Normalize the input image to the range [0, 1]
    normalized_image = (input_image - min_range_i) / denominator

    # Apply linear intensity scaling using the parameter
    transformed_image = parameter * normalized_image

    # Clip values to ensure they are within the valid range [0, 1]
    transformed_image = np.clip(transformed_image, 0.0, 1.0)

    # Denormalize the image to the original range
    transformed_image = min_range_i + (max_range_i - min_range_i) * transformed_image

    # Ensure the result is within the valid range
    transformed_image = np.clip(transformed_image, min_range_i, max_range_i)

    return transformed_image.astype(np.uint8)



def transform_image(input_image, parameters, min_range, max_range):
    transformed_image = input_image.copy()
    
    for i in range(12):
        transformed_image = apply_parameters(transformed_image, parameters[i], min_range[i], max_range[i])
    
    return transformed_image

def compute_fitness(input_image, transformed_image):
    mse = np.mean((input_image - transformed_image) ** 2)
    fitness = 1.0 / (mse + 1e-10)
    return fitness

def selection(population, fitness, num_parents):
    selected_indices = np.random.choice(len(population), size=num_parents, p=fitness/sum(fitness))
    selected_population = [population[i] for i in selected_indices]
    return selected_population

def crossover(parents):
    num_parents = len(parents)
    children = []
    
    for i in range(0, num_parents, 2):
        parent1 = parents[i]
        parent2 = parents[i + 1]
        
        crossover_point = random.randint(1, len(parent1) - 1)
        child1 = parent1[:crossover_point] + parent2[crossover_point:]
        child2 = parent2[:crossover_point] + parent1[crossover_point:]
        
        children.append(child1)
        children.append(child2)
    
    return children

def mutation(chromosomes, mutation_rate):
    for i in range(len(chromosomes)):
        for j in range(len(chromosomes[i])):
            if random.random() < mutation_rate:
                chromosomes[i][j] = 1 - chromosomes[i][j]
    return chromosomes

def genetic_algorithm(population, generations, mutation_rate, input_image, min_range, max_range):
    num_parents = 2
    for gen in range(generations):
        fitness_values = [compute_fitness(input_image, transform_image(input_image, decode_parameters(chrom, min_range, max_range), min_range, max_range)) for chrom in population]
        selected_population = selection(population, fitness_values, num_parents)
        crossed_population = crossover(selected_population)
        mutated_population = mutation(crossed_population, mutation_rate)
        population = mutated_population
    best_chromosome = population[0]
    best_parameters = decode_parameters(best_chromosome, min_range, max_range)
    return best_parameters

pop_size = 100
chromlen = 120
generations = 100
mutation_rate = 0.01

initial_population = create_initial_population(pop_size, chromlen)
input_image = cv2.imread("low_baloon.pgm", cv2.IMREAD_GRAYSCALE)
gmin = input_image.min()
gmax = input_image.max()

best_parameters = genetic_algorithm(initial_population, generations, mutation_rate, input_image, [0, 1, 0, 1, gmin, gmax, 1, 3, 0, 1, 0, 1], [0, 1, 0, 1, gmin, gmax, 1, 3, 0, 1, 0, 1])
enhanced_image = transform_image(input_image, best_parameters, [0, 1, 0, 1, gmin, gmax, 1, 3, 0, 1, 0, 1], [0, 1, 0, 1, gmin, gmax, 1, 3, 0, 1, 0, 1])
cv2.imwrite("enhanced_image.pgm", enhanced_image)
cv2.imshow('Enhanced Image', enhanced_image)
cv2.waitKey(0)
cv2.destroyAllWindows()

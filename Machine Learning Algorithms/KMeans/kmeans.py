import pandas as pd
from sklearn.decomposition import PCA
import matplotlib.pyplot as plt
import os
import math
import numpy as np
import random
import re
df = pd.read_csv(r"C:\Users\Robert\Desktop\ML\KMeans\data\concrete.csv")
pca = PCA(n_components=2)
pca_result = pca.fit_transform(df)
df_reduced = pd.DataFrame(data=pca_result, columns=['X', 'Y'])

def distance_points(point1, point2, p):
    
    distance = 0

    x1 = point1['X']
    y1 = point1['Y']

    x2 = point2['X']
    y2 = point2['Y']
    
    if p==1:
        distance = abs(x2-x1) + abs(y2-y1)
    else:
        distance1 = (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1)
        distance = math.sqrt(distance1) 
    
    return distance
    
def generate_random_points(n):
    max_X = df_reduced['X'].max()
    min_X = df_reduced['X'].min()

    max_Y = df_reduced['Y'].max()
    min_Y = df_reduced['Y'].min()

    random_points = []

    for _ in range(n):
        random_X = np.random.uniform(min_X, max_X)
        random_Y = np.random.uniform(min_Y, max_Y)
        random_points.append({'X': random_X, 'Y': random_Y})
    return random_points

def distance_to_df(point1 , p):

    distances = []

    for index, row in df_reduced.iterrows():
        distance = distance_points(point1, row, p)
        distances.append(distance)

    return pd.Series(distances, name='Distances')


def distance_to_centroids(centroids, p):
   
    distances_matrix = pd.DataFrame()

    for centroid in centroids:
        distances_series = distance_to_df(centroid, p)
        distances_matrix[f"Centroid_X:{centroid['X']}_Y:{centroid['Y']}"] = distances_series
    return distances_matrix



def closest_centroid(distances_matrix):
    
    closest_centroids = []

    for index, row in distances_matrix.iterrows():
        closest_centroid = row.idxmin()
        closest_centroids.append(closest_centroid)
    return closest_centroids


def get_clusters(closest_centroids_list):
    clusters = {}

    for point_index, centroid_index in enumerate(closest_centroids_list):
        if centroid_index not in clusters:
            clusters[centroid_index] = []

        clusters[centroid_index].append(point_index)

    return clusters

def update_centroids(clusters,centroids):

    new_centroids = []

    for centroid_label, point_indices in clusters.items():
       
        if point_indices:
            cluster_points = df_reduced.iloc[point_indices]

           
            new_centroid = {
                'X': cluster_points['X'].mean(),
                'Y': cluster_points['Y'].mean()
            }
        else:
        
            new_centroid = {
                'X': centroids[centroid_label]['X'],
                'Y': centroids[centroid_label]['Y']
            }

        new_centroids.append(new_centroid)
    return new_centroids

def kmeans_plus_initialization(n_clusters, random_seed=None):
   
    if random_seed is None:
        random_seed = random.randint(0, 500)

    np.random.seed(random_seed)

    random_sample = df_reduced.sample()
    random_result = random_sample.to_dict(orient='records')[0]
    initial_centroid = random_result
    centroids = [initial_centroid]

    for _ in range(1, n_clusters):
    
        distances = df_reduced.apply(lambda x: np.min([np.linalg.norm(np.array(x) - np.array(list(centroid.values()))) for centroid in centroids]), axis=1)
        samples = df_reduced.sample(weights=distances**2)
        selected_sample = samples.to_dict(orient='records')[0]
        next_centroid = selected_sample

        centroids.append(next_centroid)
    return centroids



def kmeans_algorithm(n_clusters, num_iterations, initialization_type):

    if initialization_type == "kmeans++":
        centroids = kmeans_plus_initialization(n_clusters)
    elif initialization_type == "random":
        centroids = generate_random_points(n_clusters)

    for _ in range(num_iterations):
        
        distances_matrix = distance_to_centroids(centroids, 2)
        closest_centroids = closest_centroid(distances_matrix)
        clusters = get_clusters(closest_centroids)
        centroids = update_centroids(clusters, centroids)

    
    final_cluster = {}
    for centroid_label, point_row in clusters.items():
        centroid_key = f"Cluster_of_{centroid_label}"
        final_cluster[centroid_key] = point_row

    return final_cluster



def kmeans_jscore(input_clusters):

    jscore = 0

    for centroid_label, point_row in input_clusters.items():
        centroid_prework = re.findall(r'-?\d+\.\d+', centroid_label)

        if centroid_prework:
            centroid_values = [float(value) for value in centroid_prework]
            centroid = {'X': centroid_values[0], 'Y': centroid_values[1]}
            cluster_points = df_reduced.iloc[point_row]
            distances = ((cluster_points['X'] - centroid['X'])**2 + (cluster_points['Y'] - centroid['Y'])**2)
            jscore += distances.sum()

    return jscore

def kmeans_multiple_initiations(n_clusters, ninit,initialization_type):

    lowest_jscore = float('inf')
    low_jscore_centroids = []
    num_iterations=30

    for _ in range(ninit):

        final_clustering = kmeans_algorithm(n_clusters, num_iterations, initialization_type)
        centroids = []

        for centroid_key, point_row in final_clustering.items():

            x_start = centroid_key.find('X:') + 2
            y_start = centroid_key.find('Y:') + 2
            x_end = centroid_key.find('_Y:')

            x = float(centroid_key[x_start:x_end])
            y = float(centroid_key[y_start:])
        
            centroids.append({'X': x, 'Y': y})


        jscore = kmeans_jscore(final_clustering)

        if jscore < lowest_jscore:
            lowest_jscore = jscore
            low_jscore_centroids = centroids

    ########
    for _ in range(num_iterations):
            
            distances_matrix = distance_to_centroids(low_jscore_centroids, 2)
            closest_centroids = closest_centroid(distances_matrix)
            clusters = get_clusters(closest_centroids)
            low_jscore_centroids = update_centroids(clusters, low_jscore_centroids)


    final_cluster1 = {}
    for centroid_index, point_row in clusters.items():
            centroid_key1 = f"Cluster_of_{centroid_index}"
            final_cluster1[centroid_key1] = point_row 

    print(final_cluster1)
    print(low_jscore_centroids)
    final_j_score = kmeans_jscore(final_cluster1)
    print(final_j_score)


n_clusters = 3
num_iterations = 100
initialization_type = "kmeans++"
kmeans_multiple_initiations(n_clusters, num_iterations, initialization_type)



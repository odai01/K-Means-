import pandas as pd
import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
from sklearn import datasets
from sklearn.cluster import KMeans
from matplotlib.patches import Ellipse

def main():
    iris = datasets.load_iris()
    iris_data = iris.data
    inertias = []  
    for k in range(1,11,1):
        model = KMeans(n_clusters=k, random_state=0)
        model.fit(iris_data)
        inertia = model.inertia_
        inertias.append(inertia)    
    plt.plot(range(1, 11), inertias)
    plt.xlabel('Number of clusters (k)')
    plt.ylabel('Inertia')
    plt.title('Elbow Method for selection of optimal K clusters')
    ##plt.annotate('Elbow Point', ((2, inertias[1])), xytext=(30, 30),textcoords='offset points', arrowprops=dict(arrowstyle='->'))
    annotation_text = 'Elbow Points'
    plt.annotate(annotation_text, xy=(2.7, 300), xytext=(0, 0),textcoords='offset points')
    plt.annotate('', xy=(2, inertias[1]), xytext=(2.7, 300),arrowprops=dict(arrowstyle='->'))
    plt.annotate('', xy=(3, inertias[2]), xytext=(2.7, 300),arrowprops=dict(arrowstyle='->'))
    circle1 = Ellipse(xy=(2, inertias[1]), width=0.5, height=25, angle=0,edgecolor='red', facecolor='none')
    circle2 = Ellipse(xy=(3, inertias[2]), width=0.5, height=25, angle=0,edgecolor='blue', facecolor='none')
    plt.gca().add_patch(circle1)
    plt.gca().add_patch(circle2)
    plt.savefig('elbow.png')
    
# end of functions
main()
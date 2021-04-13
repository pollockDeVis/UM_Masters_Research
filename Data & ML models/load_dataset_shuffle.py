# -*- coding: utf-8 -*-
"""
Created on Tue Apr 13 02:08:08 2021

@author: Pollock
"""

import numpy as np
import matplotlib.pyplot as plt 
import pandas as pd

dataset = pd.read_excel("Willow_Creek_May-Sept.xlsx")

dataset = dataset.sample(frac = 1)

trimmed_dataset =  dataset.iloc[:,[2,3,4,5,6,7]]
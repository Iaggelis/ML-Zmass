# import matplotlib.pyplot as plt
import numpy as np


def gaussPDF(x, *params):
    mu, sig = params
    f = 1.0/(np.sqrt(2.0*np.pi*sig*sig))*np.exp(-(x-mu)**2/(2.0*sig*sig))
    return f


# def negLogL(data, *params):
#     pdf = gaussPDF(data, *params)
#     if np.any(np.isnan(pdf)) or np.any(pdf <= 0):
#         return 9e99
#     return -np.sum(np.log(pdf))


data = [93.4, 89.3, 94.9, 90.7, 89.9, 89.3, 90.9, 93.8, 91.3, 92.9, 91.4, 87.8, 91.5, 89.9, 94.4, 91.3, 88.3, 91.9, 95.4, 91.1, 92.1, 91.7, 94.3, 92.0,
        89.9, 90.6, 91.7, 90.4, 92.6, 93.8, 90.9, 90.4, 89.3, 94.6, 90.6, 94.1, 89.7, 90.0, 92.8, 93.8, 93.8, 91.7, 88.2, 92.9, 86.6, 90.6, 89.9, 95.3, 92.5, 96.5]
data = np.array(data)
params = [90.0, 5.0]
initials = {'mu': 90.0, 'sig': 5.0}

mu, sig = params
print(mu, sig)
# print(gaussPDF(data, *params))

# from iminuit import Minuit
# minuit = Minuit(negLogL, errordef=0.5, **params)
# migrad = minuit.migrad()
# minuit.covariance

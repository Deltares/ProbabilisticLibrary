import numpy as np
import time

# Z-function Bligh
def bligh(m, L, c_creep, delta_H):
    Z = m * L / c_creep - delta_H
    return Z

# Z-function Hunt
def hunt(t_p, tan_alpha, h_s, h_crest, h):
    g = 9.81
    l_0 = g  * t_p * t_p
    xi = tan_alpha / np.sqrt(2 * np.pi * h_s / l_0)
    r_u = xi * h_s

    Z = h_crest - (h + r_u)
    return Z

# Z-function Hunt with 3 outputs
def hunt_3_outputs(t_p, tan_alpha, h_s, h_crest, h):
    g = 9.81
    l_0 = g  * t_p * t_p
    xi = tan_alpha / np.sqrt(2 * np.pi * h_s / l_0)
    r_u = xi * h_s

    Z = h_crest - (h + r_u)
    return Z, xi, r_u

# overtopping calculation
def overtopping(h, hm0, tm10, wave_direction, dike_normal, y_crest):

    f_shallow = 0.6778
    f_b = 4.30
    f_n = 2.30
    gamma_f = 1.0
    gamma_b = 1.0
    tan_alpha = 1/3
    g = 9.81

    angle = np.abs(wave_direction-dike_normal)
    if angle<=180.0:
        beta = angle
    else:
        beta = 360.0-angle

    if beta>110.0:
        hm0 = 0.0
        tm10 = 0.0
    
    if beta>80.0 and beta<=110.0:
        hm0 = hm0*(110.0-beta)/30.0
        tm10 = tm10*np.sqrt((110.0-beta)/30.0)

    gamma_beta0 = 1.0-0.0033*min([beta,80.0])

    l0 = g/(2*np.pi)*tm10**2
    s0 = hm0/l0

    eta_0 = tan_alpha/np.sqrt(s0)
    eta_shallow = max([7.0,eta_0])
    term = np.sqrt(g*hm0**3)

    q_b = (0.067/np.sqrt(tan_alpha))*gamma_b*eta_0*np.exp(-f_b*((y_crest-h)/hm0)*(1/(gamma_beta0*gamma_f*gamma_b*eta_0)))
    q_n = 0.2*np.exp(-f_n*((y_crest-h)/hm0)*(1/(gamma_beta0*gamma_f)))
    q_shallow = (10**(-f_shallow))*np.exp(-(y_crest-h)/(hm0*gamma_beta0*gamma_f*(0.33+0.022*eta_shallow)))

    q_0 = 0.0
    if eta_0<=5.0:
        q_0 = term*min([q_b, q_n])

    elif eta_0>=7.0:
        q_0 = term*q_shallow

    else:
        if q_n>0.0 and q_shallow>0.0:
            q_0 = term*max([q_n, np.exp(np.log(q_n)+(np.log(q_shallow)-np.log(q_n)))*(eta_0-5.0)/(7.0-5.0)])
        else:
            q_0= 0.0

    if hm0<0.05:
        q_0 = 0.0

    if h>y_crest:
        q_0 = np.nan

    return q_0

# Z-function for overtopping
def z_func_overtopping(h, hm0, tm10, wave_direction, dike_normal, y_crest, q_crit):

    if h>y_crest:
        Z = y_crest - h
    else:  
        Z = q_crit - overtopping(h, hm0, tm10, wave_direction, dike_normal, y_crest)
    
    return Z

# Z-function for overtopping, slow
class ZFunctionOvertopping():

    def __init__(self):
        pass
        # this is a placeholder for any initialization if needed
    def z_sleep(self, h, hm0, tm10, wave_direction, dike_normal, y_crest, q_crit):

        time.sleep(0.1)
        Z = z_func_overtopping(h, hm0, tm10, wave_direction, dike_normal, y_crest, q_crit)

        return Z

# critical head difference calculation according to Sellmeijer
def model_sellmeijer(k, L, d70, D):

    RD = 0.725
    RDm = 0.725
    d70m = 2.08e-4
    nu = 1.33e-6
    eta = 0.25
    theta = 37.0

    f_resistance = 1.65*eta*np.tan(theta/180*np.pi)*(RD/RDm)**0.35
    f_scale = d70m/(nu/9.81*k*L)**(1/3)*(d70/d70m)**0.39
    f_geometry = 0.91*(D/L)**(0.28/(((D/L)**2.8)-1)+0.04)

    if D==L:
        f_geometry = 1.0
    
    delta_h_c = f_resistance * f_scale * f_geometry * L
    
    return delta_h_c

# critical head difference calculation according to Sellmeijer
class Sellmeijer():

    def __init__(self):
        
        self.RD = 0.725
        self.RDm = 0.725
        self.d70m = 2.08e-4
        self.nu = 1.33e-6
        self.eta = 0.25
        self.theta = 37.0

    def critical_head(self, k, L, d70, D):

        f_resistance = 1.65*self.eta*np.tan(self.theta/180*np.pi)*(self.RD/self.RDm)**0.35
        f_scale = self.d70m/(self.nu/9.81*k*L)**(1/3)*(d70/self.d70m)**0.39
        f_geometry = 0.91*(D/L)**(0.28/(((D/L)**2.8)-1)+0.04)

        if D==L:
            f_geometry = 1.0
        
        delta_h_c = f_resistance * f_scale * f_geometry * L
        
        return delta_h_c

# simple Z-function
def linear_a_b(a, b):
    Z = 1.9 - (a+b)
    return Z

# simple Z-function
def linear_b_c(b, c):
    Z = 1.85 - (1.5 * b + 0.5 * c)
    return Z

# Z-function with arrays
def linear_arrays(const_val: float, a: list[float], b:list[float]):
    Z = const_val + sum(a) + sum(b)
    return Z

# Koppejan method
def get_bearing_capacity(z, q_clay, q_sand, D, L):
    A = 0.25 * np.pi * D**2     
    L_clay = min(L, z)
    L_sand = max(0, L - z)

    q_i = q_sand if L > z else q_sand * ((z-L) / 0.7 * A) + q_clay * (1 - (z-L)/0.7 * A)
    q_ii = q_i
    q_iii = q_sand if L_sand > 8 * A else q_sand * (L_sand/8 * A) + q_clay * (1 - L_sand/(8 * A))
    q_tip = 0.25 * q_i + 0.25 * q_ii + 0.5 * q_iii

    p_tip = A * q_tip
    p_shaft = np.pi * D * (q_clay * L_clay + q_sand * L_sand)

    p = p_tip + p_shaft
    
    return p

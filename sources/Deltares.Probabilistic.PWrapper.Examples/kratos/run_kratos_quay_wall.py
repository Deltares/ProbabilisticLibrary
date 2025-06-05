import numpy as np
import os

import KratosMultiphysics as Kratos
import KratosMultiphysics.GeoMechanicsApplication as kratos_geo
import KratosMultiphysics.StructuralMechanicsApplication as kratos_struct

import KratosMultiphysics.GeoMechanicsApplication.geomechanics_analysis as analysis
import KratosMultiphysics.GeoMechanicsApplication.prob_analysis as prob_analysis
import KratosMultiphysics.GeoMechanicsApplication.test_helper as test_helper

class zfuncwrapper_mohr_coulomb:

    def __init__(self, output_stage_number, clean_up):

        self.output_stage_number = output_stage_number
        self.clean_up = clean_up

        self.template_project_path = os.path.join(__file__, os.pardir, "Quay_Wall_4Stage_with_mohr_coulomb")
        
        stage_numbers = [1] # the wrapper of Kratos applies these values to all stages, so we can just use one stage number
        # state 0 is K0, we do not need to change that

        layers = ["PorousDomain.Parts_Solid_layer_1|1",
                  "PorousDomain.Parts_Solid_layer_1|2",
                  "PorousDomain.Parts_Solid_layer_2|1",
                  "PorousDomain.Parts_Solid_layer_2|2",
                  "PorousDomain.Parts_Solid_layer_3|1",
                  "PorousDomain.Parts_Solid_layer_3|2",
                  "PorousDomain.Parts_Solid_layer_4|1"]

        interfaces = ["PorousDomain.Layer11Interface",
                      "PorousDomain.Layer12Interface",
                      "PorousDomain.Layer21Interface",
                      "PorousDomain.Layer22Interface",
                      "PorousDomain.Layer31Interface",
                      "PorousDomain.Layer32Interface"]

        layers_variables = [kratos_geo.GEO_COHESION, 
                            kratos_geo.GEO_FRICTION_ANGLE, 
                            kratos_geo.GEO_TENSILE_STRENGTH,
                            Kratos.YOUNG_MODULUS]

        interfaces_variables = [kratos_geo.GEO_COHESION,
                                kratos_geo.GEO_FRICTION_ANGLE,
                                kratos_geo.GEO_TENSILE_STRENGTH]

        sheetpile_variables = [Kratos.YOUNG_MODULUS, 
                               Kratos.POISSON_RATIO, 
                               kratos_struct.CROSS_AREA, 
                               kratos_struct.AREA_EFFECTIVE_Y, 
                               kratos_struct.I33]
        
        self.input_parameters = []
        for variable in layers_variables:
                for stage_number in stage_numbers:
                    for layer in layers:
                        self.input_parameters.append([stage_number, layer, variable])

        for variable in interfaces_variables:
            for stage_number in stage_numbers:
                for interface in interfaces:
                    self.input_parameters.append([stage_number, interface, variable])

        for variable in sheetpile_variables:
            for stage_number in stage_numbers:
                self.input_parameters.append([stage_number, "PorousDomain.Parts_Beam_sheetpile", variable])

    def total_displacement(self, 
                           geo_cohesion: list[float], 
                           geo_friction_angle: list[float],
                           geo_tensile_strength: list[float],
                           geo_young_modulus: list[float],
                           interface_cohesion: list[float],
                           interface_friction_angle: list[float],
                           interface_tensile_strength: list[float],
                           sheetpile_young_modulus: list[float],
                           sheetpile_poisson_ratio: list[float],
                           sheetpile_thickness: list[float]) -> float:

        thickness = sheetpile_thickness + sheetpile_thickness + [val**3/12 for val in sheetpile_thickness]
        
        input_list = geo_cohesion + geo_friction_angle + geo_tensile_strength + geo_young_modulus + interface_cohesion + interface_friction_angle + interface_tensile_strength + sheetpile_young_modulus + sheetpile_poisson_ratio + thickness
        
        # Format of a single 'output_parameters'  entry = 
        # [<stage_nr>, <function you want to perform on the results (can be None)>, <get-function>, <ModelPartName>, <VariableName>, <node_id (can be None>)]
        output_parameters = [[self.output_stage_number, np.max, test_helper.get_nodal_variable, "PorousDomain", kratos_geo.TOTAL_DISPLACEMENT, None]]
        prob_analysis_instance = prob_analysis.prob_analysis(self.template_project_path, self.input_parameters, output_parameters)
        output_values = prob_analysis_instance.calculate(input_list)
        total_displacement = output_values[0]
        
        if self.clean_up:
            prob_analysis_instance.finalize()

        return np.abs(total_displacement)

    def max_bending_moment(self, 
                           geo_cohesion: list[float], 
                           geo_friction_angle: list[float],
                           geo_tensile_strength: list[float],
                           geo_young_modulus: list[float],
                           interface_cohesion: list[float],
                           interface_friction_angle: list[float],
                           interface_tensile_strength: list[float],
                           sheetpile_young_modulus: list[float],
                           sheetpile_poisson_ratio: list[float],
                           sheetpile_thickness: list[float]) -> float:
        
        thickness = sheetpile_thickness + sheetpile_thickness + [val**3/12 for val in sheetpile_thickness]

        input_list = geo_cohesion + geo_friction_angle + geo_tensile_strength + geo_young_modulus + interface_cohesion + interface_friction_angle + interface_tensile_strength + sheetpile_young_modulus + sheetpile_poisson_ratio + thickness

        # Format of a single 'output_parameters'  entry = 
        # [<stage_nr>, <function you want to perform on the results (can be None)>, <get-function>, <ModelPartName>, <VariableName>, <node_id (can be None>)]
        output_parameters = [[self.output_stage_number, np.max, test_helper.get_on_integration_points, "PorousDomain.Parts_Beam_sheetpile", kratos_struct.BENDING_MOMENT],
                             [self.output_stage_number, None, test_helper.get_on_integration_points, "PorousDomain.Parts_Beam_sheetpile", kratos_struct.BENDING_MOMENT, [410, 412]]]

        prob_analysis_instance = prob_analysis.prob_analysis(self.template_project_path, self.input_parameters, output_parameters)
        output_values = prob_analysis_instance.calculate(input_list)
        max_bending_moment = output_values[0]

        if self.clean_up:
            prob_analysis_instance.finalize()

        return max_bending_moment

class zfuncwrapper_linear:

    def __init__(self, output_stage_number, clean_up):

        self.output_stage_number = output_stage_number
        self.clean_up = clean_up

        self.template_project_path = os.path.join(__file__, os.pardir, "QuayWallModel_new")

        stage_numbers = [0] # the wrapper of Kratos applies these values to all stages, so we can just use one stage number

        layers = ["PorousDomain.Parts_Solid_layer_1|1",
                  "PorousDomain.Parts_Solid_layer_1|2",
                  "PorousDomain.Parts_Solid_layer_2|1",
                  "PorousDomain.Parts_Solid_layer_2|2",
                  "PorousDomain.Parts_Solid_layer_3|1",
                  "PorousDomain.Parts_Solid_layer_3|2",
                  "PorousDomain.Parts_Solid_layer_4|1"]
        
        interfaces = ["PorousDomain.Layer11Interface",
                      "PorousDomain.Layer12Interface",
                      "PorousDomain.Layer21Interface",
                      "PorousDomain.Layer22Interface",
                      "PorousDomain.Layer31Interface",
                      "PorousDomain.Layer32Interface"]
        
        interfaces_variables = [kratos_geo.INTERFACE_NORMAL_STIFFNESS]
        
        layers_variables = [Kratos.YOUNG_MODULUS]
        
        sheetpile_variables = [Kratos.YOUNG_MODULUS, 
                               Kratos.POISSON_RATIO, 
                               kratos_struct.CROSS_AREA, 
                               kratos_struct.AREA_EFFECTIVE_Y, 
                               kratos_struct.I33]
        
        self.input_parameters = []
        for variable in layers_variables:
                for stage_number in stage_numbers:
                    for layer in layers:
                        self.input_parameters.append([stage_number, layer, variable])

        for variable in interfaces_variables:
            for stage_number in stage_numbers:
                for interface in interfaces:
                    self.input_parameters.append([stage_number, interface, variable])

        for variable in sheetpile_variables:
            for stage_number in stage_numbers:
                self.input_parameters.append([stage_number, "PorousDomain.Parts_Beam_sheetpile", variable])

    def displacement_x(self, 
                       geo_young_modulus: list[float],
                       interface_stiffness: list[float],
                       sheetpile_young_modulus: list[float],
                       sheetpile_poisson_ratio: list[float],
                       sheetpile_thickness: list[float]) -> float:

        thickness = sheetpile_thickness + sheetpile_thickness + [val**3/12 for val in sheetpile_thickness]
        
        input_list = geo_young_modulus + interface_stiffness + sheetpile_young_modulus + sheetpile_poisson_ratio + thickness
        
        # Format of a single 'output_parameters'  entry = 
        # [<stage_nr>, <function you want to perform on the results (can be None)>, <get-function>, <ModelPartName>, <VariableName>, <node_id (can be None>)]
        output_parameters = [[self.output_stage_number, None, test_helper.get_nodal_variable, "PorousDomain", Kratos.DISPLACEMENT_X, [171]]]
        prob_analysis_instance = prob_analysis.prob_analysis(self.template_project_path, self.input_parameters, output_parameters)
        output_values = prob_analysis_instance.calculate(input_list)
        displacement_x = output_values[0]
        
        if self.clean_up:
            prob_analysis_instance.finalize()

        return displacement_x

    def max_bending_moment(self, 
                           geo_young_modulus: list[float], 
                           interface_stiffness: list[float],
                           sheetpile_young_modulus: list[float],
                           sheetpile_poisson_ratio: list[float],
                           sheetpile_thickness: list[float]) -> float:
        
        thickness = sheetpile_thickness + sheetpile_thickness + [val**3/12 for val in sheetpile_thickness]

        input_list = geo_young_modulus + interface_stiffness + sheetpile_young_modulus + sheetpile_poisson_ratio + thickness

        # Format of a single 'output_parameters'  entry = 
        # [<stage_nr>, <function you want to perform on the results (can be None)>, <get-function>, <ModelPartName>, <VariableName>, <node_id (can be None>)]
        output_parameters = [[self.output_stage_number, np.max, test_helper.get_on_integration_points, "PorousDomain.Parts_Beam_sheetpile", kratos_struct.BENDING_MOMENT],
                             [self.output_stage_number, None, test_helper.get_on_integration_points, "PorousDomain.Parts_Beam_sheetpile", kratos_struct.BENDING_MOMENT, [410, 412]]]

        prob_analysis_instance = prob_analysis.prob_analysis(self.template_project_path, self.input_parameters, output_parameters)
        output_values = prob_analysis_instance.calculate(input_list)
        max_bending_moment = output_values[0]

        if self.clean_up:
            prob_analysis_instance.finalize()

        return max_bending_moment

#wrapper = zfuncwrapper_mohr_coulomb(output_stage_number=3, clean_up=True)

#geo_cohesion = [3.0] * 7 * 1
#geo_friction_angle = [22.5] * 7 * 1
#geo_tensile_strength = [0.0] * 7 * 1
#sheetpile_young_modulus = [200.0E9] * 1
#sheetpile_poisson_ratio = [0.0] * 1
#sheetpile_thickness = [0.025] * 1

#total_displacement = wrapper.total_displacement(geo_cohesion,
#                                                geo_friction_angle,
#                                                geo_tensile_strength,
#                                                sheetpile_young_modulus,
#                                                sheetpile_poisson_ratio,
#                                                sheetpile_thickness)

#print(total_displacement)

#wrapper = zfuncwrapper_linear(output_stage_number=3, clean_up=True)

#geo_young_modulus = [500.0E5] * 7 * 1
#sheetpile_young_modulus = [200.0E9] * 1
#sheetpile_poisson_ratio = [0.0] * 1
#sheetpile_thickness = [0.03] * 1

#max_x_displacement = wrapper.max_x_displacement(geo_young_modulus,
#                                                sheetpile_young_modulus,
#                                                sheetpile_poisson_ratio,
#                                                sheetpile_thickness)

#print(max_x_displacement)
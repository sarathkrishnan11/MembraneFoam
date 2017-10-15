/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 1991-2009 OpenCFD Ltd.
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM; if not, write to the Free Software Foundation,
    Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

Class
    Foam::explicitROmembraneSoluteFvPatchScalarField

Description
    Foam::explicitROmembraneSoluteFvPatchScalarField

    this boundary condition is designed to simulate a porous baffle wall
    across which a scalar is transported. This is designed to work with the
    explicitROmembraneSoluteVelocityFvPatchVectorField. The full boundary
    condition set up with typical values is given below:
    {
        type 	explicitROmembraneSolute;
        value	uniform 0;	// initialisation value
        U	U; 		// velocity field (default value of "U")
        R	1; 		// rejection coefficient
                                //  0 - no rejection
                                //  1 - full rejection
    }
    The following parameters are set in the "constant/transportProperties"
    dictionary and are shared with the solver
    D_AB_Min        D_AB_Min        [ 0 2 -1 0 0 0 0 ]      1.45e-9;
    D_AB_Coeff      D_AB_Coeff      [ 0 2 -1 0 0 0 0 ]      1.61e-9;
    D_AB_mACoeff    D_AB_mACoeff    [ 0 0 0 0 0 0 0 ]       14;
    rho0            rho0            [ 1 -3 0 0 0 0 0 ]      997.1;
    rho_mACoeff     rho_mACoeff     [ 0 0 0 0 0 0 0 ]       694;

    Density changes across the membrane are not taken into account.
    For more details see:
    Fletcher, D. F., Wiley, D. E. (2004) A computational fluids dynamics study
    of buoyancy effects in reverse osmosis. Journal of Membrane Science 245
    (2004) pages 175-181.

SourceFiles
    explicitROmembraneSoluteFvPatchScalarField.C

\*---------------------------------------------------------------------------*/

#ifndef explicitROmembraneSoluteFvPatchScalarField_H
#define explicitROmembraneSoluteFvPatchScalarField_H

#include "fvPatchField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
               Class explicitROmembraneSoluteFvScalarPatch Declaration
\*---------------------------------------------------------------------------*/

class explicitROmembraneSoluteFvPatchScalarField
:
      public fvPatchField<scalar>
{
    // Private data

        IOdictionary transProps_;           //- Transport Properties dictionary
        word UName_;                        //- Name of the velocity field
        scalar R_;                          //- Rejection coefficient
        dimensionedScalar D_AB_Min_;        //- Diffusivity coefficient
        dimensionedScalar D_AB_Coeff_;      //- Diffusivity coefficient
        dimensionedScalar D_AB_mACoeff_;    //- Diffusivity coefficient
        dimensionedScalar rho0_;            //- Density of fluid
        dimensionedScalar rho_mACoeff_;     //- Density m_A coefficient
        List<label> fm_;                    //- Face indexing list
        Field<scalar> VIC_;                 //- valueInternalCoefficients
        Field<scalar> VBC_;                 //- valueBoundaryCoefficients
        Field<scalar> GIC_;                 //- gradientInternalCoefficients
        Field<scalar> GBC_;                 //- gradientBoundaryCoefficients

public:

    //- Runtime type information
    TypeName("explicitROmembraneSolute");


    // Constructors

        //- Construct from patch and internal field
        explicitROmembraneSoluteFvPatchScalarField
        (
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        explicitROmembraneSoluteFvPatchScalarField
        (
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping given explicitROmembraneSoluteFvPatchScalarField
        //  onto a new patch
        explicitROmembraneSoluteFvPatchScalarField
        (
            const explicitROmembraneSoluteFvPatchScalarField&,
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        explicitROmembraneSoluteFvPatchScalarField
        (
            const explicitROmembraneSoluteFvPatchScalarField&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchScalarField> clone() const
        {
            return tmp<fvPatchScalarField>
            (
                new explicitROmembraneSoluteFvPatchScalarField(*this)
            );
        }

        //- Construct as copy setting internal field reference
        explicitROmembraneSoluteFvPatchScalarField
        (
            const explicitROmembraneSoluteFvPatchScalarField&,
            const DimensionedField<scalar, volMesh>&
        );

        //- Construct and return a clone setting internal field reference
        virtual tmp<fvPatchScalarField> clone
        (
            const DimensionedField<scalar, volMesh>& iF
        ) const
        {
            return tmp<fvPatchScalarField>
            (
                new explicitROmembraneSoluteFvPatchScalarField(*this, iF)
            );
        }


    // Member functions

        // Access

            const IOdictionary& transportProperties() const { return transProps_; }     //- Transport Properties dictionary
            const word& UName() const { return UName_; }                                //- Name of the velocity field
            scalar R() const { return R_; }                                             //- Rejection coefficient
            dimensionedScalar D_AB_Min() const { return D_AB_Min_; }                    //- Minimum diffusivity
            dimensionedScalar D_AB_Coeff() const { return D_AB_Coeff_; }                //- Main diffusivity coefficient
            dimensionedScalar D_AB_mACoeff() const { return D_AB_mACoeff_; }            //- M_A diffusivity coefficient
            dimensionedScalar rho0() const { return rho0_; }                            //- Solution density
            dimensionedScalar rho_mACoeff() const { return rho_mACoeff_; }              //- Density m_A coefficient

//            virtual bool fixesValue() const
//            {
//                return true;
//            }

        // Mapping functions

            //- Map (and resize as needed) from self given a mapping object
            virtual void autoMap
            (
                const fvPatchFieldMapper&
            );

            //- Reverse map the given fvPatchField onto this fvPatchField
            virtual void rmap
            (
                const fvPatchScalarField&,
                const labelList&
            );


        // Evaluation functions

            //- Update the coefficients associated with the patch field
            virtual void updateCoeffs();

            //- Evaluate the patch field
            virtual void evaluate
            (
                const Pstream::commsTypes commsType=Pstream::blocking
            );

            //- Return the matrix diagonal coefficients corresponding to the
            //  evaluation of the value of this patchField with given weights
            virtual tmp<Field<scalar> > valueInternalCoeffs
            (
                const tmp<scalarField>&
            ) const;

            //- Return the matrix source coefficients corresponding to the
            //  evaluation of the value of this patchField with given weights
            virtual tmp<Field<scalar> > valueBoundaryCoeffs
            (
                const tmp<scalarField>&
            ) const;

            //- Return the matrix diagonal coefficients corresponding to the
            //  evaluation of the gradient of this patchField
            virtual tmp<Field<scalar> > gradientInternalCoeffs() const;

            //- Return the matrix source coefficients corresponding to the
            //  evaluation of the gradient of this patchField
            virtual tmp<Field<scalar> > gradientBoundaryCoeffs() const;


        //- Write
        virtual void write(Ostream&) const;

    // Member operators
/*
        virtual void operator=(const UList<scalar>&) {}

        virtual void operator=(const fvPatchField<scalar>&) {}
        virtual void operator+=(const fvPatchField<scalar>&) {}
        virtual void operator-=(const fvPatchField<scalar>&) {}
        virtual void operator*=(const fvPatchField<scalar>&) {}
        virtual void operator/=(const fvPatchField<scalar>&) {}

        virtual void operator+=(const Field<scalar>&) {}
        virtual void operator-=(const Field<scalar>&) {}

        virtual void operator*=(const Field<scalar>&) {}
        virtual void operator/=(const Field<scalar>&) {}

        virtual void operator=(const scalar&) {}
        virtual void operator+=(const scalar&) {}
        virtual void operator-=(const scalar&) {}
        virtual void operator*=(const scalar) {}
        virtual void operator/=(const scalar) {}
*/
private:
    //- initialise the object and compute the cell-index mapping
    void calcFaceMapping();

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //

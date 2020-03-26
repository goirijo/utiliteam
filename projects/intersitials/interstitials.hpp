
bool MatrixComparison(Eigen::Matrix3d Matrix1, Eigen::Matrix3d Matrix2)
{
	return Matrix1.isApprox(Matrix2);
}


bool VectorComparison(Eigen::Vector3d Vector1, Eigen::Vector3d Vector2)
{
	return Vector1.isApprox(Vector2);
}

struct compare_mat{
	compare_mat(Eigen::Matrix3d Matrix1) : Matrix1(Matrix1) {}
	bool operator()(Eigen::Matrix3d Matrix2) const {
		return MatrixComparison(Matrix1, Matrix2);
	}

	private:
	Eigen::Matrix3d Matrix1;
};

struct compare_vect{
	compare_vect(Eigen::Vector3d Vector1) : Vector1(Vector1) {}
	bool operator()(Eigen::Vector3d Vector2) const {
		return VectorComparison(Vector1, Vector2);
	}

	private:
	Eigen::Vector3d Vector1;
};

class factor_group{
	public: 

		Eigen::Matrix3d SymOp;
		Eigen::Vector3d translation;
		factor_group(Eigen::Matrix3d s, Eigen::Vector3d tau){
			translation=tau;
			SymOp=s;
		}

		auto get_translation()
		{
			return translation;
		}

		auto get_SymOp()
		{
			return SymOp;
		}

#define CATCH_CONFIG_RUNNER
#include <catch.hpp>

#include "scenegraph.hpp"
#include "node.hpp"
#include "geometrynode.hpp"
#include "cameranode.hpp"

TEST_CASE("node","[node]") {

	SECTION("constructor, getter") {
		glm::mat4 mat1 = glm::mat4(1.0);
		glm::mat4 mat2 = glm::mat4(0.0);

		Node root{"root"};
		Node child1{&root, "child1", mat1, mat2};
		Node child2{"child2"};
		child2.setParent(&root);
		child2.setLocalTransform(mat1);
		child2.setWorldTransform(mat2);

		//Testing getting and setting Parent Relation
		REQUIRE(root.getParent() == nullptr);
		REQUIRE(child1.getParent() == &root);
		REQUIRE(child2.getParent() == &root);
		child2.setParent(&child1);
		REQUIRE(child2.getParent() == &child1);

		//Testing getting and setting Child Relation
		REQUIRE(root.getChildren("child1") == &child1);
		REQUIRE(root.getChildren("nd") == nullptr);

		Node newchild{"newchild"};
		root.addChildren(&newchild);
		REQUIRE(root.getChildren("newchild") == &newchild);
		Node removedNode = *(root.removeChildren("newchild"));
		REQUIRE(root.getChildren("newchild") == nullptr);

		//Path and Depth
		std::cout<<"Paths \n";
		std::cout<<"root: "<<root.getPath()<<"\n";
		std::cout<<"child1: "<<child1.getPath()<<"\n";
		std::cout<<"child2: "<<child2.getPath()<<"\n";
		std::cout<<"newchild: "<<removedNode.getPath()<<"\n";

		REQUIRE(root.getDepth() == 0);
		REQUIRE(child1.getDepth() == 1);
		REQUIRE(child2.getDepth() == 2);

		Node c{"c"};
		root.addChildren(&c);
		std::cout<<c.getPath();
		std::cout<<c.getDepth();
	}
}


int main(int argc, char *argv[])
{
  return Catch::Session().run(argc, argv);
}
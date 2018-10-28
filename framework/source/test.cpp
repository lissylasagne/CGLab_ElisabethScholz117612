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
		
		Node node1{nullptr, "node1", mat1, mat2};
		Node node2{&node1, "node2", mat1, mat2};
		Node node3{nullptr, "node3", mat1, mat2};
		
		REQUIRE(node1.getParent() == nullptr);
		REQUIRE(node2.getParent() == &node1);
		node3.setParent(&node2);
		REQUIRE(node3.getParent() == &node2);

		REQUIRE(node1.getChildren("node2") == &node2);
		REQUIRE(node2.getChildren("node3") == &node3);

		REQUIRE(*(node1.getChildrenList().begin()) == &node2);
		REQUIRE(*(node2.getChildrenList().begin()) == &node3);

		REQUIRE(node1.getName() == "node1");
		REQUIRE(node2.getName() == "node2");
		REQUIRE(node3.getName() == "node3");

		//REQUIRE(node1.getPath() ==);
		REQUIRE(node1.getDepth() == 0);
		REQUIRE(node2.getDepth() == 1);
		REQUIRE(node3.getDepth() == 2);

		REQUIRE(node1.getLocalTransform() == mat1);
		REQUIRE(node1.getWorldTransform() == mat2);

		node1.setLocalTransform(mat2);
		node1.setWorldTransform(mat1);
		REQUIRE(node1.getLocalTransform() == mat2);
		REQUIRE(node1.getWorldTransform() == mat1);
	}
}


int main(int argc, char *argv[])
{
  return Catch::Session().run(argc, argv);
}
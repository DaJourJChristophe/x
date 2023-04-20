import java.util.Stack;

class TreeNode
{
  char ch;

  TreeNode left;
  TreeNode right;

  TreeNode(char c)
  {
    ch = c;
    left = null;
    right = null;
  }

  TreeNode(char c, TreeNode lt, TreeNode rt)
  {
    ch = c;
    left = lt;
    right = rt;
  }
}

class Tree
{
  TreeNode root;

  Tree()
  {
    root = null;
  }

  private void _traverse(TreeNode node)
  {
    if (node == null)
    {
      return;
    }

    _traverse(node.left);
    System.out.print(node.ch);
    _traverse(node.right);
  }

  public void traverse()
  {
    _traverse(root);
  }
}

public class Main
{
  private static boolean isSymbol(char c)
  {
    return c == '+' || c == '*' || c == '-' || c == '/';
  }

  public static int getPrecedence(char c)
  {
    switch (c)
    {
      case '*': return 3;
      case '/': return 2;
      case '+': return 1;
      case '-': return 0;

      default:
    }

    return -1;
  }

  public static void infixToPostfix(String s)
  {
    Stack<Character> symbol = new Stack<>();
    Stack<TreeNode> nodes = new Stack<>();
    Tree tree = new Tree();

    for (char c : s.toCharArray())
    {
      if (isSymbol(c))
      {
        if (!symbol.isEmpty() && (getPrecedence(symbol.peek()) > getPrecedence(c)))
        {
          nodes.push(new TreeNode(symbol.pop(), nodes.pop(), nodes.pop()));
        }

        symbol.push(c);
      }
      else if (Character.isDigit(c))
      {
        nodes.push(new TreeNode(c));
      }
    }

    while (!symbol.isEmpty())
    {
      nodes.push(new TreeNode(symbol.pop(), nodes.pop(), nodes.pop()));
    }

    tree.root = nodes.pop();

    tree.traverse();
    System.out.println();
  }

  public static void main(String[] args)
  {
    Main.infixToPostfix("1 + 2 + 3");
    Main.infixToPostfix("1 + 2 * 3");
    Main.infixToPostfix("1 * 2 + 3");
  }
}

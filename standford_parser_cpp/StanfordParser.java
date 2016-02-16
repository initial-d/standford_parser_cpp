import java.util.Collection;
import java.util.List;
import java.io.StringReader;

import edu.stanford.nlp.process.TokenizerFactory;
import edu.stanford.nlp.process.LexedTokenFactory;
import edu.stanford.nlp.trees.TreebankLanguagePack;
import edu.stanford.nlp.process.CoreLabelTokenFactory;
import edu.stanford.nlp.process.DocumentPreprocessor;
import edu.stanford.nlp.process.PTBTokenizer;
import edu.stanford.nlp.ling.CoreLabel;
import edu.stanford.nlp.ling.HasWord;
import edu.stanford.nlp.ling.Sentence;
import edu.stanford.nlp.trees.*;
import edu.stanford.nlp.parser.lexparser.LexicalizedParser;

class StanfordParser {

    public static String parse(String sent) {
        TokenizerFactory<CoreLabel> tokenizerFactory =
                PTBTokenizer.factory(new CoreLabelTokenFactory(), "");
        List<CoreLabel> rawWords =
                tokenizerFactory.getTokenizer(new StringReader(sent)).tokenize();
        Tree parse = lp.apply(rawWords);

        TreebankLanguagePack tlp = lp.getOp().langpack();
        GrammaticalStructureFactory gsf = tlp.grammaticalStructureFactory();
        GrammaticalStructure gs = gsf.newGrammaticalStructure(parse);
        List<TypedDependency> tdl = gs.typedDependenciesCCprocessed();

        String result = parse.toString() + "\t" + tdl.toString();
        return result;
    }
    
    public static String[] options = { "-maxLength", "80", "-retainTmpSubcategories" ,"-nthreads", "4"};
    public static LexicalizedParser lp = LexicalizedParser.loadModel("edu/stanford/nlp/models/lexparser/englishPCFG.ser.gz", options);
}

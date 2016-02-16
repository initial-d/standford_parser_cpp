import edu.stanford.nlp.ling.HasWord;
import edu.stanford.nlp.ling.Sentence;
import edu.stanford.nlp.ling.TaggedWord;
import edu.stanford.nlp.parser.nndep.DependencyParser;
import edu.stanford.nlp.process.DocumentPreprocessor;
import edu.stanford.nlp.tagger.maxent.MaxentTagger;
import edu.stanford.nlp.trees.GrammaticalStructure;
import edu.stanford.nlp.process.PTBTokenizer;
import edu.stanford.nlp.ling.CoreLabel;
import edu.stanford.nlp.process.TokenizerFactory;
import edu.stanford.nlp.process.CoreLabelTokenFactory;
import java.io.StringReader;
import edu.stanford.nlp.process.Morphology;
import java.io.IOException;
import java.lang.Exception;
import java.util.List;
import edu.stanford.nlp.trees.TypedDependency;

public class NNDepParser {
    public static void main(String[] args) throws IOException {
        System.out.println(parse("nothing of it ?"));
    }
    public static String parse(String sent) {
        String result = "";


        List<CoreLabel> rawWords =
                tokenizerFactory.getTokenizer(new StringReader(sent)).tokenize();

        try {
            List<TaggedWord> tagged = tagger.tagSentence(rawWords);
            GrammaticalStructure gs = parser.predict(tagged);
            List<TypedDependency> tdl = gs.typedDependenciesCCprocessed();
            result += "[";
            for(TaggedWord tw : tagged) {
                result += "(";
                result += tw.word();
                result += " ";
                result += tw.tag();
                result += ")";
            }
            result += "]\t";
            result += tdl;
        } catch (Exception e) {
            System.err.println(e.getMessage());
            return result;
        }
        return result;
    }

    public static String lemma(String word, String tag) {
        try {
            return mp.lemma(word, tag);
        }catch (Exception e){
            return word;
        }
    }

    public static Morphology mp = new Morphology();
    public static String modelPath = "edu/stanford/nlp/models/parser/nndep/english_SD.gz";
    public static String taggerPath = "edu/stanford/nlp/models/pos-tagger/english-left3words/english-left3words-distsim.tagger";

    public static TokenizerFactory<CoreLabel> tokenizerFactory = PTBTokenizer.factory(new CoreLabelTokenFactory(), "");

    public static MaxentTagger tagger = new MaxentTagger(taggerPath);
    public static DependencyParser parser = DependencyParser.loadFromModelFile(modelPath);
}
